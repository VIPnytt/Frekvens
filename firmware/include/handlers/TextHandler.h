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

    uint8_t height = 0;
    uint8_t tracking = 1;
    uint8_t width = 0;

    std::string text{};

    template <typename T>
        requires std::is_unsigned_v<T>
    [[nodiscard]] uint8_t calcMsbMax(std::span<const T> bitmap) const;

    bool nextCodepoint(size_t &index, uint32_t &out) const;

    static std::array<char, 5> encode(uint32_t codepoint);
};
