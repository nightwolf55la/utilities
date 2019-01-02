#pragma once

#include "generator_iterator.h"

namespace util::gen {
    template<class UnderlyingIterator>
    struct EnumerateState {
        int64_t idx_;
        UnderlyingIterator iter_;

        template <std::size_t N>
        decltype(auto) get() const {
            if      constexpr (N == 0) return idx_;
            else if constexpr (N == 1) return *iter_;
        }
    };

    template<class Iterable>
    class enumerate {
    public:
        Iterable iterable_;
        int64_t starting_idx_ = 0;

    public:
        using State = EnumerateState<decltype(std::begin(iterable_))>;
        State state_{starting_idx_, std::begin(iterable_)};

    public:
        constexpr enumerate(const enumerate&) = delete;
        constexpr enumerate(enumerate&&) = delete;
        constexpr enumerate& operator=(const enumerate&) = delete;
        constexpr enumerate& operator=(enumerate&&) = delete;

    public:
        using Iterator = GeneratorIterator<enumerate>;
        constexpr Iterator begin() { return Iterator{*this}; }
        constexpr GeneratorEnd end() { return {}; }

    public:
        constexpr State& operator*() { return state_; }
        constexpr void operator++() { ++state_.idx_; ++state_.iter_; }
        constexpr explicit operator bool() const { return state_.iter_ != std::end(iterable_); }
    };

    template<class Iterable>
    enumerate(Iterable&& iterable) -> enumerate<decltype(iterable)>;

    template<class Iterable>
    enumerate(Iterable&& iterable, int64_t) -> enumerate<decltype(iterable)>;
}

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmismatched-tags"
#endif

namespace std {
    template<std::size_t N, class UnderlyingIterator>
    struct tuple_element<N, util::gen::EnumerateState<UnderlyingIterator>> {
        using type = decltype(std::declval<util::gen::EnumerateState<UnderlyingIterator>>().template get<N>());
    };

    template<class UnderlyingIterator>
    struct tuple_size<util::gen::EnumerateState<UnderlyingIterator>> : std::integral_constant<std::size_t, 2> {
        // Empty
    };
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
