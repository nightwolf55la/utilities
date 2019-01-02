#pragma once

namespace util::gen {
    struct GeneratorEnd {};

    template<class Generator>
    class GeneratorIterator {
    public:
        constexpr explicit GeneratorIterator(Generator& generator) : generator_(generator) { }
        constexpr GeneratorIterator(const GeneratorIterator&) = default;
        constexpr GeneratorIterator(GeneratorIterator&&) noexcept = default;
        constexpr GeneratorIterator& operator=(const GeneratorIterator&) = default;
        constexpr GeneratorIterator& operator=(GeneratorIterator&&) noexcept = default;

    public:
        constexpr decltype(auto) operator*() { return *generator_; }
        constexpr GeneratorIterator& operator++() { ++generator_; return *this; }
        constexpr bool operator!=(const GeneratorEnd&) const { return generator_.operator bool(); }

    private:
        Generator& generator_;
    };
}
