#pragma once

#include <string>
#include <string_view>

#include "modules/FontModule.h"

class TextHandler
{
private:
    static constexpr std::string_view _name = "TextHandler";

    const std::string text;

    FontModule *font = nullptr;

    uint8_t
        height = 0,
        i = 0,
        tracking = 1,
        width = 0;

    uint8_t calcMsbMax(const FontModule::Symbol &character);

    bool nextCodepoint(uint32_t &out);

    static const char *encode(uint32_t codepoint, char *out);

public:
    TextHandler(std::string text, FontModule *font);

    void draw(uint8_t brightness = UINT8_MAX);
    void draw(int16_t x, int8_t y, uint8_t brightness = UINT8_MAX);

    uint8_t getHeight() const;
    uint8_t getWidth() const;
};
