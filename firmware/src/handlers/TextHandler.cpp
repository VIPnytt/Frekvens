#include "handlers/TextHandler.h"

#include "config/constants.h"        // NOLINT(misc-include-cleaner)
#include "services/DisplayService.h" // NOLINT(misc-include-cleaner)

TextHandler::TextHandler(std::string text, const FontModule &font) : text(text), font(&font)
{
    if (text.length())
    {
        size_t index = 0;
        {
            int8_t yMax = 0; // NOLINT(misc-const-correctness)
            int8_t yMin = 0;
            for (uint32_t codepoint = 0; nextCodepoint(index, codepoint);)
            {
                FontModule::Symbol character{font.getChar(codepoint)};
                std::visit(
                    [&](auto &&bitmap)
                    {
                        if (!bitmap.empty())
                        {
                            yMax = max<int8_t>(yMax, bitmap.size() + character.offsetY);
                        }
                    },
                    character.bitmap);
                yMin = min<int8_t>(yMin, character.offsetY);
            }
            height = yMax - yMin;
        }
        tracking = static_cast<uint8_t>(ceilf(height / Display.getRatio() / 10.0F));
        {
            width = 0;
            size_t index = 0;
            for (uint32_t codepoint = 0; nextCodepoint(index, codepoint);)
            {
                FontModule::Symbol character{font.getChar(codepoint)};
                std::visit(
                    [&](auto &&bitmap)
                    {
                        if (!bitmap.empty())
                        {
                            width += calcMsbMax(bitmap) + 1 + character.offsetX + tracking;
                        }
                        else if (character.offsetX > 0)
                        {
                            width += character.offsetX + tracking;
                        }
                        else
                        {
                            ESP_LOGV(font->name, "missing symbol 0x%X %s", codepoint, encode(codepoint).data());
                        }
                    },
                    character.bitmap);
            }
            if (width > tracking)
            {
                width -= tracking;
            }
        }
    }
}

void TextHandler::draw(uint8_t brightness) const
{
    draw(max(0, (GRID_COLUMNS - width) / 2), (GRID_ROWS - height) / 2, brightness);
}

void TextHandler::draw(int16_t x, int8_t y, uint8_t brightness) const
{
    size_t index = 0;
    for (uint32_t codepoint = 0; nextCodepoint(index, codepoint);)
    {
        FontModule::Symbol character{font->getChar(codepoint)};
        std::visit(
            [&](auto &&bitmap)
            {
                const uint8_t _height = bitmap.size();
                if (_height != 0)
                {
                    const uint8_t msbMax{calcMsbMax(bitmap)};
                    for (uint16_t _x = 0; _x <= msbMax; ++_x)
                    {
                        for (uint8_t _y = 0; _y < _height; ++_y)
                        {
                            if ((x + character.offsetX + _x) >= 0 && (x + character.offsetX + _x) < GRID_COLUMNS &&
                                (int16_t)(y + height - _height - character.offsetY + _y) >= 0 &&
                                (int16_t)(y + height - _height - character.offsetY + _y) < GRID_ROWS &&
                                (bitmap[_y] >> (msbMax - _x)) & 1)
                            {
                                Display.setPixel(x + character.offsetX + _x,
                                                 y + height - _height - character.offsetY + _y,
                                                 brightness);
                            }
                        }
                    }
                    x += msbMax + 1 + character.offsetX + tracking;
                }
                else if (character.offsetX > 0)
                {
                    x += character.offsetX + tracking;
                }
            },
            character.bitmap);
    }
}

uint8_t TextHandler::getHeight() const { return height; }

uint8_t TextHandler::getWidth() const { return width; }

bool TextHandler::nextCodepoint(size_t &index, uint32_t &buffer) const
{
    if (index >= text.length())
    {
        return false;
    }
    const uint8_t byte{static_cast<uint8_t>(text[index])}; // NOLINT(cppcoreguidelines-init-variables)
    index++;
    if (byte <= 0x7F)
    {
        buffer = byte;
        return true;
    }
    uint8_t bytes = 0;
    if ((byte & 0xE0U) == 0xC0U)
    {
        buffer = byte & 0x1FU;
        bytes = 1;
    }
    else if ((byte & 0xF0U) == 0xE0U)
    {
        buffer = byte & 0x0FU;
        bytes = 2;
    }
    else if ((byte & 0xF8U) == 0xF0U)
    {
        buffer = byte & 0x07U;
        bytes = 3;
    }
    else
    {
        buffer = 0xFFFDU;
        return true;
    }
    while (bytes-- && index < text.length())
    {
        const uint8_t _byte{static_cast<uint8_t>(text[index])};
        index++;
        if ((_byte & 0xC0U) != 0x80U)
        {
            buffer = 0xFFFDU;
            break;
        }
        buffer = (buffer << 6U) | (_byte & 0x3FU);
    }
    return true;
}

template <typename T>
    requires std::is_unsigned_v<T>
uint8_t TextHandler::calcMsbMax(std::span<const T> bitmap) const
{
    uint8_t msbMax = 0; // NOLINT(misc-const-correctness)
    for (const T bitset : bitmap)
    {
        if (bitset != 0)
        {
            msbMax = max(msbMax, static_cast<uint8_t>(std::bit_width(bitset) - 1));
        }
    }
    return msbMax;
}

std::array<char, 5> TextHandler::encode(uint32_t codepoint)
{
    std::array<char, 5> out{};
    if (codepoint <= 0x7F)
    {
        out.at(0) = static_cast<char>(codepoint);
    }
    else if (codepoint <= 0x7FF)
    {
        out.at(0) = static_cast<char>(0xC0 | (codepoint >> 6U));
        out.at(1) = static_cast<char>(0x80 | (codepoint & 0x3FU));
    }
    else if (codepoint <= 0xFFFF)
    {
        out.at(0) = static_cast<char>(0xE0 | (codepoint >> 12U));
        out.at(1) = static_cast<char>(0x80 | ((codepoint >> 6U) & 0x3FU));
        out.at(2) = static_cast<char>(0x80 | (codepoint & 0x3FU));
    }
    else if (codepoint <= 0x10FFFF)
    {
        out.at(0) = static_cast<char>(0xF0 | (codepoint >> 18U));
        out.at(1) = static_cast<char>(0x80 | ((codepoint >> 12U) & 0x3FU));
        out.at(2) = static_cast<char>(0x80 | ((codepoint >> 6U) & 0x3FU));
        out.at(3) = static_cast<char>(0x80 | (codepoint & 0x3FU));
    }
    return out;
}
