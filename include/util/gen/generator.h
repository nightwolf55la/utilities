#pragma once

#include "generator_iterator.h"

namespace util::gen {
    template<class Impl>
    class Generator : public Impl {
    private:
        using Iterator = GeneratorIterator<Generator>;

    public:
        Iterator begin() { return Iterator(*this); }
        GeneratorEnd end() { return {}; }

        decltype(auto) operator*() { return Impl::operator*(); }
        void operator++() { Impl::operator++(); } // TODO: Should this return a Generator& (*this)
        explicit operator bool() const { return Impl::operator bool(); }

    public:
        Generator(const Generator&) = delete;
        Generator(Generator&&) = delete;
        Generator& operator=(const Generator&) = delete;
        Generator& operator=(Generator&&) = delete;
    };
}