#include <catch2/catch.hpp>

#include <queue>
#include <util/gen/generator.h>

namespace {
    enum class GenFunc {
        DEREFERENCE,
        INCREMENT,
        BOOL
    };

    struct GeneratorImpl {
        size_t operator*() { funcs_called.push(GenFunc::DEREFERENCE); return val; }
        void operator++() { funcs_called.push(GenFunc::INCREMENT); }
        operator bool() const { funcs_called.push(GenFunc::BOOL); return val < 3; }

        size_t val = 0;
        mutable std::queue<GenFunc> funcs_called{};
    };

    using TestGenerator = util::gen::Generator<GeneratorImpl>;
}

TEST_CASE("Generator Begin/End") {
    TestGenerator generator{};
    CHECK(std::is_same_v<decltype(generator.end()), util::gen::GeneratorEnd>);
    CHECK(!std::is_same_v<decltype(generator.begin()), decltype(generator.end())>);
}

TEST_CASE("Generator Operators") {
    TestGenerator generator{};
    CHECK(generator.funcs_called.empty());

    generator.operator++();
    CHECK(generator.funcs_called.size() == 1);
    CHECK(generator.funcs_called.back() == GenFunc::INCREMENT);

    generator.operator*();
    CHECK(generator.funcs_called.size() == 2);
    CHECK(generator.funcs_called.back() == GenFunc::DEREFERENCE);

    generator.operator bool();
    CHECK(generator.funcs_called.size() == 3);
    CHECK(generator.funcs_called.back() == GenFunc::BOOL);
}

TEST_CASE("Generator Iterator") {
    TestGenerator generator{};
    auto it = generator.begin();
    CHECK(generator.funcs_called.empty());

    it.operator++();
    CHECK(generator.funcs_called.size() == 1);
    CHECK(generator.funcs_called.back() == GenFunc::INCREMENT);

    it.operator*();
    CHECK(generator.funcs_called.size() == 2);
    CHECK(generator.funcs_called.back() == GenFunc::DEREFERENCE);

    it.operator!=(util::gen::GeneratorEnd{});
    CHECK(generator.funcs_called.size() == 3);
    CHECK(generator.funcs_called.back() == GenFunc::BOOL);
}
