#include <catch2/catch.hpp>

#include <vector>
#include <array>
#include <deque>
#include <list>
#include <forward_list>
#include <util/gen/enumerate.h>

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
        [[maybe_unused]] Iterable i{}; // Clang 6.0 compilation segfaults without this line
        auto&& iterable = get_iterable<Iterable, CONST, RVALUE>();
        for(auto&&[idx, value] : util::gen::enumerate{get_iterable<Iterable, CONST, RVALUE>()}) {
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
    void test_iterable_wo_starting_index() {
        test<Iterable, false, false>();
        test<Iterable, true, false>();
        test<Iterable, false, true>();
        test<Iterable, true, true>();
    }


    template<class Iterable, bool CONST, bool RVALUE>
    void test_start_idx(int64_t starting_idx) {
        auto&& iterable = get_iterable<Iterable, CONST, RVALUE>();
        for(auto&&[idx, value] : util::gen::enumerate{get_iterable<Iterable, CONST, RVALUE>(), starting_idx}) {
            using ValueType = decltype(value);
            static_assert(std::is_lvalue_reference_v<ValueType>);
            static_assert(std::is_const_v<std::remove_reference_t<ValueType>> == CONST);

            CHECK(idx == value + starting_idx);
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
    void test_iterable_w_starting_idx(int64_t starting_idx) {
        test_start_idx<Iterable, false, false>(starting_idx);
        test_start_idx<Iterable, true, false>(starting_idx);
        test_start_idx<Iterable, false, true>(starting_idx);
        test_start_idx<Iterable, true, true>(starting_idx);
    }

    template<class Iterable>
    void test_iterable() {
        test_iterable_wo_starting_index<Iterable>();
        test_iterable_w_starting_idx<Iterable>(0);
        test_iterable_w_starting_idx<Iterable>(7);
        test_iterable_w_starting_idx<Iterable>(-8);
    }
}

TEST_CASE("enumerate std::vector") {
    test_iterable<std::vector<int>>();
}

TEST_CASE("enumerate std::deque") {
    test_iterable<std::deque<int>>();
}

TEST_CASE("enumerate std::list") {
    test_iterable<std::list<int>>();
}

TEST_CASE("enumerate std::forward_list") {
    test_iterable<std::forward_list<int>>();
}

TEST_CASE("enumerate std::array") {
    test_iterable<std::array<int, 5>>();
}

TEST_CASE("enumerate c-style array") {
    SECTION("LValue") {
        int arr[5] = {0, 1, 2, 3, 4};
        for(auto&&[idx, value] : util::gen::enumerate{arr}) {
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
        for(auto&&[idx, value] : util::gen::enumerate{arr}) {
            using ValueType = decltype(value);
            static_assert(std::is_lvalue_reference_v<ValueType>);
            static_assert(std::is_const_v<std::remove_reference_t<ValueType>>);

            CHECK(idx == value);
        }
    }
}

#ifdef __GNUC__
#ifndef __clang__
TEST_CASE("enumerate initializer list") {
    SECTION("LValue") {
        for(auto&&[idx, value] : util::gen::enumerate{{0,1,2,3,4}}) {
            using ValueType = decltype(value);
            static_assert(std::is_lvalue_reference_v<ValueType>);
            static_assert(std::is_const_v<std::remove_reference_t<ValueType>>);

            CHECK(idx == value);
        }
    }
}
#endif
#endif
