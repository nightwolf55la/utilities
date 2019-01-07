#pragma once

#include <cstdint>
#include <utility>
#include "generator_iterator.h"

namespace util::gen {
    ////////////////////////////////////////////////////////////////////////////////
    // Forward Declarations
    ////////////////////////////////////////////////////////////////////////////////
    template<size_t IDX, class... Iterables>
    struct ZipStorage;

    template<size_t IDX, class... Iterables>
    struct ZipState;

    ////////////////////////////////////////////////////////////////////////////////
    // zip - This class behaves like a Generator, but does not inherit from
    // Generator because of a clang bug related to rvalue reference lifetime
    // extensions in base classes.  So the Generator behavior is emulated instead.
    ////////////////////////////////////////////////////////////////////////////////
    template<class... Iterables>
    class zip {
    public:
        //------------------------------------------------------------------------------
        // Initialization variables - Can only be set via aggregate initialization
        //------------------------------------------------------------------------------
        using Storage = ZipStorage<0, Iterables...>;
        Storage storage_;

    public:
        //------------------------------------------------------------------------------
        // State of the generator - It's public to allow for aggregate initialization of
        // the above variables, but this should be treated as private.
        //------------------------------------------------------------------------------
        using State = ZipState<0, Iterables...>;
        State state_{storage_};

    public:
        //------------------------------------------------------------------------------
        // Implicit Constructors - All Deleted
        //------------------------------------------------------------------------------
        constexpr zip() = delete;
        constexpr zip(const zip&) = delete;
        constexpr zip(zip&&) = delete;
        constexpr zip& operator=(const zip&) = delete;
        constexpr zip& operator=(zip&&) = delete;

    public:
        //------------------------------------------------------------------------------
        // Begin and End - Enables usage in ranged-for
        //------------------------------------------------------------------------------
        using Iterator = GeneratorIterator<zip>;
        constexpr Iterator begin() { return Iterator{*this}; }
        constexpr GeneratorEnd end() { return {}; }

    public:
        //------------------------------------------------------------------------------
        // operators - Emulating the generator class's operators
        //------------------------------------------------------------------------------
        constexpr State& operator*() { return state_; }
        constexpr zip& operator++() { ++state_; return *this; }
        constexpr explicit operator bool() const { return state_.iterator != std::end(storage_.iterable); }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // Deduction guides
    //      zip{some_iterable, more_iterables...}
    ////////////////////////////////////////////////////////////////////////////////

    template<class RequiredIterable, class... OptionalIterables>
    zip(RequiredIterable&&  required_iterable, OptionalIterables&&... optional_iterables)
        -> zip<decltype(required_iterable), decltype(optional_iterables)...>;

    ////////////////////////////////////////////////////////////////////////////////
    // ZipStorage - Holds references to the passed in iterators
    ////////////////////////////////////////////////////////////////////////////////
    template<size_t IDX, class CurrentIterable>
    struct ZipStorage<IDX, CurrentIterable> {
        CurrentIterable iterable;

        constexpr auto begin() { return std::begin(iterable); }
    };

    template<size_t IDX, class CurrentIterable, class... RemainingIterables>
    struct ZipStorage<IDX, CurrentIterable, RemainingIterables...> {
        CurrentIterable iterable;

        using NextStorage = ZipStorage<IDX+1, RemainingIterables...>;
        NextStorage next_storage;

        constexpr auto begin() { return std::begin(iterable); }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // ZipState - This represents the state of the zip generator
    ////////////////////////////////////////////////////////////////////////////////
    template<size_t IDX, class CurrentIterable>
    struct ZipState<IDX, CurrentIterable> {
        // Construct from a ZipStorage matching this depth
        explicit constexpr ZipState(ZipStorage<IDX, CurrentIterable>& storage)
            : iterator(storage.begin())
        {
            // Nothing
        }

        // Increment this iterator
        constexpr void operator++() { ++iterator; }

        // Enables structured bindings
        template <std::size_t N>
        constexpr decltype(auto) get() const {
            if constexpr(N == IDX) { return *iterator; }
        }

        // Member Variables
        using Iterator = decltype(std::declval<ZipStorage<IDX, CurrentIterable>>().begin());
        Iterator iterator;
    };

    template<size_t IDX, class CurrentIterable, class... RemainingIterables>
    struct ZipState<IDX, CurrentIterable, RemainingIterables...> {
        // Construct from a ZipStorage matching this depth
        explicit constexpr ZipState(ZipStorage<IDX, CurrentIterable, RemainingIterables...>& storage)
            : iterator(std::begin(storage.iterable))
            , next_state(storage.next_storage)
        {
            // Nothing
        }

        // Increment this iterator and all remaining iterators
        constexpr void operator++() { ++iterator; ++next_state; }

        // Enables structured bindings
        template <std::size_t N>
        constexpr decltype(auto) get() const {
            if constexpr(N == IDX) { return *iterator; }
            else { return next_state.template get<N>(); }
        }

        // Member Variables
        using Iterator = decltype(std::declval<ZipStorage<IDX, CurrentIterable>>().begin());
        Iterator iterator;

        using NextState = ZipState<IDX+1, RemainingIterables...>;
        NextState next_state;
    };
}

// Need to do this since array and tuple define tuple_element and tuple_size as a class and a struct
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmismatched-tags"
#endif

////////////////////////////////////////////////////////////////////////////////
// tuple_element and tuple_size specializations for EnumerateState's structured bindings
////////////////////////////////////////////////////////////////////////////////
namespace std {
    template<std::size_t N, class... Iterables>
    struct tuple_element<N, util::gen::ZipState<0, Iterables...>> {
        using type = decltype(std::declval<util::gen::ZipState<0, Iterables...>>().template get<N>());
    };

    template<class... Iterables>
    struct tuple_size<util::gen::ZipState<0, Iterables...>> : std::integral_constant<std::size_t, sizeof...(Iterables)> {
        // Empty
    };
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
