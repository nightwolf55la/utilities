#pragma once

#include "generator.h"

namespace util::gen {
    ////////////////////////////////////////////////////////////////////////////////
    // RangeImpl - This class is the implementation for the range generator
    ////////////////////////////////////////////////////////////////////////////////
    class RangeImpl {
    public:
        //------------------------------------------------------------------------------
        // Constructor - Must be initialized with a begin, end, and step
        //------------------------------------------------------------------------------
        constexpr RangeImpl(int64_t begin, int64_t end, int64_t step)
            : val_(begin)
            , step_(step)
            , comparison_mod_(step < 0 ? -1 : 1)
            , modded_end_(end * comparison_mod_)
        {
            // Nothing
        }

    public:
        //------------------------------------------------------------------------------
        // operators - Provides the state, advances the generator, and determines completion
        //------------------------------------------------------------------------------
        constexpr int64_t operator*() { return val_; }
        constexpr RangeImpl& operator++() { val_ += step_; return *this; }
        constexpr explicit operator bool() const { return (val_ * comparison_mod_) < modded_end_; }

    private:
        //------------------------------------------------------------------------------
        // Member Variables
        //------------------------------------------------------------------------------
        int64_t val_;
        const int64_t step_;
        const int64_t comparison_mod_;
        const int64_t modded_end_;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // Define the Range generator
    ////////////////////////////////////////////////////////////////////////////////
    using Range = Generator<RangeImpl>;

    ////////////////////////////////////////////////////////////////////////////////
    // range function shortcuts - begin is inclusive, end is exclusive - [begin, end)
    //      range(end) - begin is 0 and step is 1
    //      range(begin, end) - step is 1
    //      range(begin, end, step) - begin is 0 and step is 1
    ////////////////////////////////////////////////////////////////////////////////
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
