#pragma once

namespace util {
    template<class>
    constexpr bool always_false() {
        return false;
    }

    template<auto>
    constexpr bool always_false() {
        return false;
    }
}
