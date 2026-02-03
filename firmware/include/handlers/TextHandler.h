#pragma once

#include "modules/FontModule.h"

#include <string>
#include <string_view>

class TextHandler
{
private:
    static constexpr std::string_view _name = "TextHandler";

    std::string text = "";

    FontModule *font = nullptr;

    uint8_t height = 0;
    uint8_t i = 0;
    uint8_t tracking = 1;
    uint8_t width = 0;

    [[nodiscard]] uint8_t calcMsbMax(const FontModule::Symbol &character) const;

    bool nextCodepoint(uint32_t &out);

    static const char *encode(uint32_t codepoint, char *out);

public:
    explicit TextHandler(std::string text, FontModule *font);

    void draw(uint8_t brightness = UINT8_MAX);
    void draw(int16_t x, int8_t y, uint8_t brightness = UINT8_MAX);

    [[nodiscard]] uint8_t getHeight() const;
    [[nodiscard]] uint8_t getWidth() const;
};
