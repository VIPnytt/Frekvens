#pragma once

#include "modules/FontModule.h"
#include "modules/HandlerModule.h"

#include <array>
#include <string>
#include <string_view>

class TextHandler final : public HandlerModule
{
public:
    explicit TextHandler(std::string text, const FontModule &font);

    void draw(uint8_t brightness = UINT8_MAX) const;
    void draw(int16_t x, int8_t y, uint8_t brightness = UINT8_MAX) const;

    [[nodiscard]] uint8_t getHeight() const;
    [[nodiscard]] uint8_t getWidth() const;

private:
    const FontModule *font;

    uint8_t height{0U};
    uint8_t tracking{1U};
    uint8_t width{0U};

    std::string text{};

    template <typename T>
        requires std::is_unsigned_v<T>
    [[nodiscard]] uint8_t calcMsbMax(std::span<const T> bitmap) const;

    bool nextCodepoint(size_t &index, char32_t &out) const;

    static std::array<char, 5U> encode(char32_t codepoint);
};
