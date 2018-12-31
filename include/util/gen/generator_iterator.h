#pragma once

namespace util::gen {
    struct GeneratorEnd {};

    template<class Generator>
    class GeneratorIterator {
    public:
        explicit GeneratorIterator(Generator& generator) : generator_(generator) { }
        GeneratorIterator(const GeneratorIterator&) = default;
        GeneratorIterator(GeneratorIterator&&) noexcept = default;
        GeneratorIterator& operator=(const GeneratorIterator&) = default;
        GeneratorIterator& operator=(GeneratorIterator&&) noexcept = default;

    public:
        decltype(auto) operator*() { return *generator_; }
        GeneratorIterator& operator++() { ++generator_; return *this; }
        bool operator!=(const GeneratorEnd&) const { return generator_.operator bool(); }

    private:
        Generator& generator_;
    };
}
