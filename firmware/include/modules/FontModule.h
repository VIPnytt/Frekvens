#pragma once

#include <cstdint>
#include <vector>

class FontModule
{
protected:
    explicit FontModule(const char *name) : name(name) {};

public:
    virtual ~FontModule() = default;

    FontModule(const FontModule &) = delete;
    FontModule &operator=(const FontModule &) = delete;
    FontModule(FontModule &&) = delete;
    FontModule &operator=(FontModule &&) = delete;

    struct Symbol
    {
        const std::vector<uint8_t> bitmap{};
        const int8_t offsetX = 0;
        const int8_t offsetY = 0;
    };

    struct SymbolExtended
    {
        const wchar_t hex = 0;
        const Symbol symbol;
    };

    const char *const name;

    [[nodiscard]] virtual Symbol getChar(uint32_t character) const = 0;
};
