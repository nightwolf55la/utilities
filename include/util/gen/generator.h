#pragma once

namespace util::gen {
    struct GeneratorEnd {};

    template<class Impl>
    class Generator : public Impl {
    private:
        class Iterator {
        public:
            explicit Iterator(Generator& generator) : generator_(generator) { }
            Iterator(const Iterator&) = default;
            Iterator(Iterator&&) noexcept = default;
            Iterator& operator=(const Iterator&) = default;
            Iterator& operator=(Iterator&&) noexcept = default;

        public:
            decltype(auto) operator*() { return *generator_; }
            Iterator& operator++() { ++generator_; return *this; }
            bool operator!=(const GeneratorEnd&) const { return generator_.operator bool(); }

        private:
            Generator& generator_;
        };

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