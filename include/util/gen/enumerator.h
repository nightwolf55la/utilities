#pragma once

#include "generator_iterator.h"
#include "../always_false.h"

namespace util::gen {
    template<class UnderlyingIterator>
    struct EnumeratorState {
        int64_t idx_;
        UnderlyingIterator iter_;

        template <std::size_t N>
        decltype(auto) get() const {
            if      constexpr (N == 0) return idx_;
            else if constexpr (N == 1) return *iter_;
        }
    };

    template<class Iterable>
    class Enumerator {
    public:
        Iterable iterable_;
        int64_t starting_idx_ = 0;

    public:
        using State = EnumeratorState<decltype(std::begin(iterable_))>;
        State state_{starting_idx_, std::begin(iterable_)};

    public:
        Enumerator(const Enumerator&) = delete;
        Enumerator(Enumerator&&) = delete;
        Enumerator& operator=(const Enumerator&) = delete;
        Enumerator& operator=(Enumerator&&) = delete;

    public:
        using Iterator = GeneratorIterator<Enumerator>;
        Iterator begin() { return Iterator{*this}; }
        GeneratorEnd end() { return {}; }

    public:
        State& operator*() { return state_; }
        void operator++() { ++state_.idx_; ++state_.iter_; }
        operator bool() const { return state_.iter_ != std::end(iterable_); }
    };

    template<class Iterable>
    Enumerator(Iterable&& iterable) -> Enumerator<decltype(iterable)>;

    template<class Iterable>
    Enumerator(Iterable&& iterable, int64_t) -> Enumerator<decltype(iterable)>;

    template<class Iterable>
    auto enumerate(Iterable&& iterable) {
        if constexpr(std::is_rvalue_reference_v<decltype(iterable)>) {
            static_assert(always_false_v<Iterable>);
        } else {
            return Enumerator{iterable};
        }
    }

    template<class Iterable>
    auto enumerate(Iterable&& iterable, int64_t starting_idx) {
        if constexpr(std::is_rvalue_reference_v<decltype(iterable)>) {
            static_assert(always_false_v<Iterable>);
        } else {
            return Enumerator{iterable, starting_idx};
        }
    }
}

#if defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmismatched-tags"
#endif

namespace std {
    template<std::size_t N, class UnderlyingIterator>
    struct tuple_element<N, util::gen::EnumeratorState<UnderlyingIterator>> {
        using type = decltype(std::declval<util::gen::EnumeratorState<UnderlyingIterator>>().template get<N>());
    };

    template<class UnderlyingIterator>
    struct tuple_size<util::gen::EnumeratorState<UnderlyingIterator>> : std::integral_constant<std::size_t, 2> {
        // Empty
    };
}

#if defined(__clang__)
#pragma GCC diagnostic pop
#endif
