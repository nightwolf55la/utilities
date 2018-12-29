#include <catch2/catch.hpp>

#include <vector>
#include <array>
#include <deque>
#include <list>
#include <forward_list>
#include <util/gen/enumerator.h>

namespace {
    template<class Iterable, bool CONST>
    using IterableTypeWConst =
        std::conditional_t<
            CONST,
            std::add_const_t<Iterable>,
            Iterable
        >;

    template<class Iterable, bool RVALUE>
    using IterableTypeWRValue =
        std::conditional_t<
            RVALUE,
            Iterable,
            std::add_lvalue_reference_t<Iterable>
        >;

    template<class Iterable, bool CONST, bool RVALUE>
    using IterableType = IterableTypeWRValue<IterableTypeWConst<Iterable, CONST>, RVALUE>;

    template<class Iterable, bool CONST, bool RVALUE>
    IterableType<Iterable, CONST, RVALUE> get_iterable() {
        if constexpr(RVALUE) {
            return {{0, 1, 2, 3, 4}};
        } else {
            static Iterable iterable;
            iterable = Iterable{{0, 1, 2, 3, 4}};
            return iterable;
        }
    }

    template<class Iterable, bool CONST, bool RVALUE>
    void test() {
        auto&& iterable = get_iterable<Iterable, CONST, RVALUE>();
        for(auto&&[idx, value] : util::gen::Enumerator{get_iterable<Iterable, CONST, RVALUE>()}) {
            using ValueType = decltype(value);
            static_assert(std::is_lvalue_reference_v<ValueType>);
            static_assert(std::is_const_v<std::remove_reference_t<ValueType>> == CONST);

            CHECK(idx == value);
            if constexpr(!CONST && !RVALUE) {
                value = 10;
            }
        }

        if constexpr(!CONST && !RVALUE) {
            for(auto&& value : iterable) {
                CHECK(value == 10);
            }
        }
    }

    template<class Iterable>
    void test_iterable() {
        test<Iterable, false, false>();
        test<Iterable, true, false>();
        test<Iterable, false, true>();
        test<Iterable, true, true>();
    }
}

TEST_CASE("Enumerator std::vector") {
    test_iterable<std::vector<int>>();
}

TEST_CASE("Enumerator std::deque") {
    test_iterable<std::deque<int>>();
}

TEST_CASE("Enumerator std::list") {
    test_iterable<std::list<int>>();
}

TEST_CASE("Enumerator std::forward_list") {
    test_iterable<std::forward_list<int>>();
}

TEST_CASE("Enumerator std::array") {
    test_iterable<std::array<int, 5>>();
}

TEST_CASE("Enumerator c-style array") {
    SECTION("LValue") {
        int arr[5] = {0, 1, 2, 3, 4};
        for(auto&&[idx, value] : util::gen::Enumerator{arr}) {
            using ValueType = decltype(value);
            static_assert(std::is_lvalue_reference_v<ValueType>);
            static_assert(!std::is_const_v<std::remove_reference_t<ValueType>>);

            CHECK(idx == value);
            value = 10;
        }

        for(auto&& value : arr) {
            CHECK(value == 10);
        }
    }

    SECTION("Const LValue") {
        const int arr[5] = {0, 1, 2, 3, 4};
        for(auto&&[idx, value] : util::gen::Enumerator{arr}) {
            using ValueType = decltype(value);
            static_assert(std::is_lvalue_reference_v<ValueType>);
            static_assert(std::is_const_v<std::remove_reference_t<ValueType>>);

            CHECK(idx == value);
        }
    }
}

TEST_CASE("Enumerator starting index") {
    std::vector<int> vec = get_iterable<std::vector<int>, false, true>();

    auto test = [&vec](int64_t starting_idx) {
        for(auto&&[idx, value] : util::gen::Enumerator{vec, starting_idx}) {
            using ValueType = decltype(value);
            static_assert(std::is_lvalue_reference_v<ValueType>);
            static_assert(!std::is_const_v<std::remove_reference_t<ValueType>>);

            CHECK(idx == value + starting_idx);
            value = idx;
        }

        for(auto&&[idx, value] : util::gen::Enumerator{vec, starting_idx}) {
            CHECK(idx == value);
        }
    };

    SECTION("Zero") {
        test(0);
    }

    SECTION("Positive") {
        test(7);
    }

    SECTION("Negative") {
        test(-8);
    }
}

TEST_CASE("Enumerator enumerate shortcut") {
    SECTION("Default starting idx") {
        std::vector<int> vec = get_iterable<std::vector<int>, false, true>();
        for(auto&&[idx, value] : util::gen::enumerate(vec)) {
            using ValueType = decltype(value);
            static_assert(std::is_lvalue_reference_v<ValueType>);
            static_assert(!std::is_const_v<std::remove_reference_t<ValueType>>);

            CHECK(idx == value);
            value = 10;
        }

        for(int value : vec) {
            CHECK(value == 10);
        }
    }

    SECTION("Starting idx") {
        std::vector<int> vec = get_iterable<std::vector<int>, false, true>();
        for(auto&&[idx, value] : util::gen::enumerate(vec, 10)) {
            using ValueType = decltype(value);
            static_assert(std::is_lvalue_reference_v<ValueType>);
            static_assert(!std::is_const_v<std::remove_reference_t<ValueType>>);

            CHECK(idx == value + 10);
            value = idx;
        }

        for(auto&&[idx, value] : util::gen::enumerate(vec, 10)) {
            CHECK(idx == value);
        }
    }
}
