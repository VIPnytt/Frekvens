#pragma once

#include <Arduino.h>

#include "modules/FontModule.h"

class TextHandler
{
private:
    static constexpr std::string_view _name = "TextHandler";

    const String text;

    FontModule *font;

    size_t utf8Index = 0;

    uint8_t
        height = 0,
        width = 0,
        spacing = 1;

    uint8_t calcMsbMax(const FontModule::Symbol &character);

    bool nextCodepoint(uint32_t &out);

    static const char *encodeUtf8(uint32_t codepoint, char *out);

public:
    TextHandler(String text, FontModule *font);

    void draw(uint8_t brightness = UINT8_MAX);
    void draw(int16_t x, int8_t y, uint8_t brightness = UINT8_MAX);

    uint8_t getHeight() const;
    uint8_t getWidth() const;
};
