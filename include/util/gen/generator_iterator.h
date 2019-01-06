#pragma once

namespace util::gen {
    ////////////////////////////////////////////////////////////////////////////////
    // GeneratorEnd - This serves as a sentinel for when a generator is finished
    ////////////////////////////////////////////////////////////////////////////////
    struct GeneratorEnd {};

    ////////////////////////////////////////////////////////////////////////////////
    // GeneratorIterator - This is common iterator behavior used by generators
    ////////////////////////////////////////////////////////////////////////////////
    template<class Generator>
    class GeneratorIterator {
    public:
        //------------------------------------------------------------------------------
        // Constructors - All implicit construction except default is allowed
        //              - Must be constructed with a Generator reference
        //              - This keeps the iterator small, which isn't a requirement, but
        //                it seems to be an expectation at this point
        //------------------------------------------------------------------------------
        constexpr explicit GeneratorIterator(Generator& generator) : generator_(generator) { }
        constexpr GeneratorIterator(const GeneratorIterator&) = default;
        constexpr GeneratorIterator(GeneratorIterator&&) noexcept = default;
        constexpr GeneratorIterator& operator=(const GeneratorIterator&) = default;
        constexpr GeneratorIterator& operator=(GeneratorIterator&&) noexcept = default;

    public:
        //------------------------------------------------------------------------------
        // operators
        // operator* - Calls the generator operator*
        // operator++ - Calls the generator operator++, but returns itself
        // operator!= - Only defined for the Generator sentinel, calls the generators operator bool
        //------------------------------------------------------------------------------
        constexpr decltype(auto) operator*() { return *generator_; }
        constexpr GeneratorIterator& operator++() { ++generator_; return *this; }
        constexpr bool operator!=(const GeneratorEnd&) const { return generator_.operator bool(); }
        constexpr bool operator==(const GeneratorEnd& end) const { return !operator !=(end); }

    private:
        //------------------------------------------------------------------------------
        // Member variables
        //------------------------------------------------------------------------------
        Generator& generator_;
    };
}
