#pragma once

#include "generator_iterator.h"

namespace util::gen {
    template<class Impl>
    class Generator : public Impl {
    private:
        using Iterator = GeneratorIterator<Generator>;

    public:
        constexpr Iterator begin() { return Iterator(*this); }
        constexpr GeneratorEnd end() { return {}; }

        constexpr decltype(auto) operator*() { return Impl::operator*(); }
        constexpr void operator++() { Impl::operator++(); } // TODO: Should this return a Generator& (*this)
        constexpr explicit operator bool() const { return Impl::operator bool(); }

    public:
        constexpr Generator(const Generator&) = delete;
        constexpr Generator(Generator&&) = delete;
        constexpr Generator& operator=(const Generator&) = delete;
        constexpr Generator& operator=(Generator&&) = delete;
    };
}