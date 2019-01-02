#pragma once

#include "generator.h"

namespace util::gen {
    class RangeImpl {
    public:
        constexpr RangeImpl(int64_t begin, int64_t end, int64_t step)
            : val_(begin)
            , step_(step)
            , comparison_mod_(step < 0 ? -1 : 1)
            , modded_end_(end * comparison_mod_)
        {
            // Nothing
        }

    public:
        constexpr int64_t operator*() { return val_; }
        constexpr void operator++() { val_ += step_; }
        constexpr explicit operator bool() const { return (val_ * comparison_mod_) < modded_end_; }

    private:
        int64_t val_;
        const int64_t step_;
        const int64_t comparison_mod_;
        const int64_t modded_end_;
    };

    using Range = Generator<RangeImpl>;

    constexpr Range range(int64_t end) {
        return Range{{0, end, 1}};
    }

    constexpr Range range(int64_t begin, int64_t end) {
        return Range{{begin, end, 1}};
    }

    constexpr Range range(int64_t begin, int64_t end, int64_t step) {
        return Range{{begin, end, step}};
    }
}
