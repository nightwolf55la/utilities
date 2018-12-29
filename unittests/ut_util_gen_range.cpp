#include <catch2/catch.hpp>
#include <util/gen/range.h>

TEST_CASE("Range - End") {
    using namespace util::gen;

    auto test = [](int64_t end) {
        int64_t count = 0;
        int64_t next = 0;
        for(int64_t i : range(end)) {
            CHECK(i == next++);
            count++;
        }
        CHECK(next == end);
        CHECK(count == end);
    };

    test(7);
    test(10);
    test(0);
}

TEST_CASE("Range - Begin/End") {
    using namespace util::gen;

    auto test = [](int64_t begin, int64_t end) {
        int64_t count = 0;
        int64_t next = begin;
        for(int64_t i : range(begin, end)) {
            CHECK(i == next++);
            count++;
        }
        CHECK(next == end);
        CHECK(count == end - begin);
    };

    test(0, 7);
    test(3, 15);
    test(-3, 9);
    test(-8, -2);
    test(-8, 0);
    test(0, 0);
}

TEST_CASE("Range - Begin/End/Step") {
    using namespace util::gen;

    auto test = [](int64_t begin, int64_t end, int64_t step) {
        int64_t count = 0;
        int64_t next = begin;
        for(int64_t i : range(begin, end, step)) {
            CHECK(i == next);
            next += step;
            count++;
        }
        // TODO: Fix this check
        //CHECK(next - end < step);
        if((end - begin) % step == 0) {
            CHECK(count == (end - begin) / step);
        } else {
            CHECK(count == (end - begin) / step + 1);
        }
        CHECK(begin + step * count == next);
    };

    test(0, 8, 1);
    test(0, 8, 2);
    test(0, 7, 3);
    test(3, 15, 2);
    test(-3, 9, 3);
    test(-8, -2, 6);
    test(1, 5, 5);
    test(0, 0, 3);
    test(5, 2, -1);
    test(6, -2, -3);
    test(-4, -9, -2);
}
