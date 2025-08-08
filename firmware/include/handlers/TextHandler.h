#pragma once

#include <Arduino.h>

#include "modules/FontModule.h"

class TextHandler
{
private:
    static constexpr std::string_view name = "TextHandler";

    const String text;

    FontModule *font;

    uint8_t
        height = 0,
        width = 0,
        spacing = 1;

public:
    TextHandler(String text, FontModule *font);

    void draw(uint8_t brightness = UINT8_MAX);
    void draw(int16_t x, int8_t y, uint8_t brightness = UINT8_MAX);

    uint8_t getHeight() const;
    uint8_t getWidth() const;
};
