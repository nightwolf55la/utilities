#pragma once

#include "generator_iterator.h"

namespace util::gen {
    ////////////////////////////////////////////////////////////////////////////////
    // Generator - This class encapsulates the common code of generator style
    // classes and enforces a common structure / behavior.
    ////////////////////////////////////////////////////////////////////////////////
    template<class Impl>
    class Generator : public Impl {
    private:
        //------------------------------------------------------------------------------
        // Types
        //------------------------------------------------------------------------------
        using Iterator = GeneratorIterator<Generator>;

    public:
        //------------------------------------------------------------------------------
        // begin/end - standard iterator access and enable use in ranged-for loops
        //------------------------------------------------------------------------------
        constexpr Iterator begin() { return Iterator(*this); }
        constexpr GeneratorEnd end() { return {}; }

        //------------------------------------------------------------------------------
        // operators
        // operator* - This should return the current state of the generator
        // operator++ - This should advance the generator. Note only the prefix++ is provided.
        //              Due to the generator having one state and not being copyable
        //              a postfix++ doesn't make sense and would only provide behavior
        //              that is inconsistent with a regular postfix++ operator.
        // operator bool - This should return true while the generator can still be advanced.
        //                 Once the generator should not be advanced anymore, this should return false
        //------------------------------------------------------------------------------
        constexpr decltype(auto) operator*() { return Impl::operator*(); }
        constexpr Generator& operator++() { Impl::operator++(); return *this; }
        constexpr explicit operator bool() const { return Impl::operator bool(); }

    public:
        //------------------------------------------------------------------------------
        // Implicit Constructors/assignment - All deleted
        // Due to the nature of a generator, moving or copying doesn't make sense
        //------------------------------------------------------------------------------
        constexpr Generator(const Generator&) = delete;
        constexpr Generator(Generator&&) = delete;
        constexpr Generator& operator=(const Generator&) = delete;
        constexpr Generator& operator=(Generator&&) = delete;
    };
}