#pragma once

#include <stdint.h>
#include <vector>

class FontModule
{
protected:
    FontModule(const char *const name) : name(name) {};

public:
    FontModule(const FontModule &) = delete;
    FontModule &operator=(const FontModule &) = delete;

    struct Symbol
    {
        const std::vector<uint8_t> bitmap;
        const int8_t
            offsetX,
            offsetY;
    };

    struct SymbolExtended
    {
        const wchar_t hex;
        const Symbol symbol;
    };

    const char *const name;

    virtual Symbol getChar(wchar_t character);
};
