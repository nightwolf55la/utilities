#include <catch2/catch.hpp>

#include <optional>
#include <vector>
#include <array>
#include <deque>
#include <list>
#include <forward_list>
#include <util/gen/zip.h>

TEST_CASE("zip - one iterable") {
    std::vector<int> vec_i{0,1,2,3};

    std::optional<int> last_int;
    for(auto&&[i] : util::gen::zip{vec_i}) {
        if(!last_int) {
            CHECK(i == 0);
        } else {
            CHECK(*last_int + 1 == i);
        }
        last_int = i;
    }
    CHECK(last_int == 3);
}

TEST_CASE("zip - two iterables") {
    std::vector<int> vec_i{0,1,2,3};
    std::list<char> list_c{'a', 'b', 'c', 'd'};

    std::optional<int> last_int;
    for(auto&&[i, c] : util::gen::zip{vec_i, list_c}) {
        if(!last_int) {
            CHECK(i == 0);
        } else {
            CHECK(*last_int + 1 == i);
        }
        last_int = i;

        switch(i) {
            case 0: CHECK(c == 'a'); break;
            case 1: CHECK(c == 'b'); break;
            case 2: CHECK(c == 'c'); break;
            case 3: CHECK(c == 'd'); break;
            default: FAIL(); break;
        }
    }
    CHECK(last_int == 3);
}

TEST_CASE("zip - many iterables") {
    std::vector<int> vec{0,1,2,3};
    std::list<char> list{'a', 'b', 'c', 'd'};
    std::forward_list<char> flist{'a', 'b', 'c', 'd'};
    std::deque<char> deque{'a', 'b', 'c', 'd'};
    std::array<char, 4> array{'a', 'b', 'c', 'd'};
    char c_array[] = {'a', 'b', 'c', 'd'};

    std::optional<int> last_int;
    for(auto&&[i, list_v, flist_v, deque_v, arr_v, carr_v] : util::gen::zip{vec, list, flist, deque, array, c_array}) {
        if(!last_int) {
            CHECK(i == 0);
        } else {
            CHECK(*last_int + 1 == i);
        }
        last_int = i;

        switch(i) {
            case 0: CHECK(list_v == 'a'); CHECK(flist_v == 'a'); CHECK(deque_v == 'a'); CHECK(arr_v == 'a'); CHECK(carr_v == 'a'); break;
            case 1: CHECK(list_v == 'b'); CHECK(flist_v == 'b'); CHECK(deque_v == 'b'); CHECK(arr_v == 'b'); CHECK(carr_v == 'b'); break;
            case 2: CHECK(list_v == 'c'); CHECK(flist_v == 'c'); CHECK(deque_v == 'c'); CHECK(arr_v == 'c'); CHECK(carr_v == 'c'); break;
            case 3: CHECK(list_v == 'd'); CHECK(flist_v == 'd'); CHECK(deque_v == 'd'); CHECK(arr_v == 'd'); CHECK(carr_v == 'd'); break;
            default: FAIL(); break;
        }
    }
    CHECK(last_int == 3);
}

TEST_CASE("zip - constness") {
    const std::vector<int> vec_i{0,1,2,3};
    std::list<char> list_c{'a', 'b', 'c', 'd'};

    std::optional<int> last_int;
    for(auto&&[i, c] : util::gen::zip{vec_i, list_c}) {
        using VecValueType = decltype(i);
        static_assert(std::is_lvalue_reference_v<VecValueType>);
        static_assert(std::is_const_v<std::remove_reference_t<VecValueType>>);

        using CharValueType = decltype(c);
        static_assert(std::is_lvalue_reference_v<CharValueType>);
        static_assert(!std::is_const_v<std::remove_reference_t<CharValueType>>);

        if(!last_int) {
            CHECK(i == 0);
        } else {
            CHECK(*last_int + 1 == i);
        }
        last_int = i;

        switch(i) {
            case 0: CHECK(c == 'a'); break;
            case 1: CHECK(c == 'b'); break;
            case 2: CHECK(c == 'c'); break;
            case 3: CHECK(c == 'd'); break;
            default: FAIL(); break;
        }
        c = 'e';
    }
    CHECK(last_int == 3);

    for(char c : list_c) {
        CHECK(c == 'e');
    }
}

TEST_CASE("zip - rvalues") {
    auto get_vec = []() -> const std::vector<int> {
        return std::vector<int>{0,1,2,3};
    };

    auto get_list = []() -> std::list<char> {
        return std::list<char>{'a', 'b', 'c', 'd'};
    };

    std::optional<int> last_int;
    for(auto&&[i, c] : util::gen::zip{get_vec(), get_list()}) {
        using VecValueType = decltype(i);
        static_assert(std::is_lvalue_reference_v<VecValueType>);
        static_assert(std::is_const_v<std::remove_reference_t<VecValueType>>);

        using CharValueType = decltype(c);
        static_assert(std::is_lvalue_reference_v<CharValueType>);
        static_assert(!std::is_const_v<std::remove_reference_t<CharValueType>>);

        if(!last_int) {
            CHECK(i == 0);
        } else {
            CHECK(*last_int + 1 == i);
        }
        last_int = i;

        switch(i) {
            case 0: CHECK(c == 'a'); break;
            case 1: CHECK(c == 'b'); break;
            case 2: CHECK(c == 'c'); break;
            case 3: CHECK(c == 'd'); break;
            default: FAIL(); break;
        }
        c = 'e';
    }
    CHECK(last_int == 3);
}

TEST_CASE("zip - different lengths") {
    std::vector<int> vec_1{0,1,2,3};
    std::vector<int> vec_2{0,1,2};
    std::list<char> list_1{'a', 'b', 'c', 'd'};

    std::optional<int> last_int;
    for(auto&&[i_1, i_2, c_1] : util::gen::zip{vec_1, vec_2, list_1}) {
        CHECK(i_1 == i_2);
        if(!last_int) {
            CHECK(i_1 == 0);
        } else {
            CHECK(*last_int + 1 == i_1);
        }
        last_int = i_1;

        switch(i_1) {
            case 0: CHECK(c_1 == 'a'); break;
            case 1: CHECK(c_1 == 'b'); break;
            case 2: CHECK(c_1 == 'c'); break;
            case 3: FAIL(); break;
            default: FAIL(); break;
        }
    }
    CHECK(last_int == 2);
}
