#pragma once

namespace util {
    template<class>
    struct always_false {
        static constexpr bool value = false;
    };

    template<class T>
    static constexpr bool always_false_v = always_false<T>::value;
}
