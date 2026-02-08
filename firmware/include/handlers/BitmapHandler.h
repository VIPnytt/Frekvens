#pragma once

#include <string_view>
#include <vector>

class BitmapHandler
{
private:
    static constexpr std::string_view _name = "BitmapHandler";

    const std::vector<uint16_t> bitmap = {};

    uint8_t height = 0;
    uint8_t width = 0;

public:
    explicit BitmapHandler(std::vector<uint16_t> bitmap);

    void draw(uint8_t brightness = UINT8_MAX);
    void draw(uint8_t x, uint8_t y, uint8_t brightness = UINT8_MAX);
    [[nodiscard]] uint8_t getHeight() const;
    [[nodiscard]] uint8_t getWidth() const;
};
