#include "handlers/TextHandler.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "services/DisplayService.h"

TextHandler::TextHandler(std::string text, FontModule *font) : text(text), font(font)
{
    if (text.length())
    {
        {
            uint8_t yMax = 0;
            uint8_t yMin = 0;
            for (uint32_t codepoint = 0; nextCodepoint(codepoint);)
            {
                FontModule::Symbol character = font->getChar(codepoint);
                if (!character.bitmap.empty())
                {
                    yMax = max<uint8_t>(yMax, character.bitmap.size() + character.offsetY);
                }
                yMin = min<int8_t>(yMin, character.offsetY);
            }
            height = yMax - yMin;
        }
        tracking = static_cast<uint8_t>(ceilf(height / Display.getRatio() / 10.0F));
        {
            i = 0;
            width = 0;
            for (uint32_t codepoint = 0; nextCodepoint(codepoint);)
            {
                FontModule::Symbol character = font->getChar(codepoint);
                if (!character.bitmap.empty())
                {
                    width += calcMsbMax(character) + 1 + character.offsetX + tracking;
                }
                else if (character.offsetX > 0)
                {
                    width += character.offsetX + tracking;
                }
                else
                {
                    ESP_LOGV(font->name, "missing symbol 0x%X %s", codepoint, encode(codepoint).data());
                }
            }
            if (width > tracking)
            {
                width -= tracking;
            }
        }
    }
}

void TextHandler::draw(uint8_t brightness)
{
    draw(max(0, (GRID_COLUMNS - width) / 2), (GRID_ROWS - height) / 2, brightness);
}

void TextHandler::draw(int16_t x, int8_t y, uint8_t brightness)
{
    i = 0;
    for (uint32_t codepoint = 0; nextCodepoint(codepoint);)
    {
        FontModule::Symbol character = font->getChar(codepoint);
        const uint8_t _height = character.bitmap.size();
        if (_height != 0)
        {
            const uint8_t msbMax = calcMsbMax(character); // NOLINT(cppcoreguidelines-init-variables)
            for (uint16_t _x = 0; _x <= msbMax; ++_x)
            {
                for (uint8_t _y = 0; _y < _height; ++_y)
                {
                    if ((x + character.offsetX + _x) >= 0 && (x + character.offsetX + _x) < GRID_COLUMNS &&
                        (int16_t)(y + height - _height - character.offsetY + _y) >= 0 &&
                        (int16_t)(y + height - _height - character.offsetY + _y) < GRID_ROWS &&
                        (character.bitmap[_y] >> (msbMax - _x)) & 1)
                    {
                        Display.setPixel(
                            x + character.offsetX + _x, y + height - _height - character.offsetY + _y, brightness);
                    }
                }
            }
            x += msbMax + 1 + character.offsetX + tracking;
        }
        else if (character.offsetX > 0)
        {
            x += character.offsetX + tracking;
        }
    }
}

uint8_t TextHandler::getHeight() const { return height; }

uint8_t TextHandler::getWidth() const { return width; }

bool TextHandler::nextCodepoint(uint32_t &buffer)
{
    if (i >= text.length())
    {
        return false;
    }
    const uint8_t byte = static_cast<uint8_t>(text[i]); // NOLINT(cppcoreguidelines-init-variables)
    i++;
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
    while (bytes-- && i < text.length())
    {
        const uint8_t cont = text[i]; // NOLINT(cppcoreguidelines-init-variables)
        i++;
        if ((cont & 0xC0U) != 0x80U)
        {
            buffer = 0xFFFDU;
            break;
        }
        buffer = (buffer << 6U) | (cont & 0x3FU);
    }
    return true;
}

uint8_t TextHandler::calcMsbMax(const FontModule::Symbol &character) const
{
    uint8_t msbMax = 0; // NOLINT(misc-const-correctness)
    for (const uint8_t bitset : character.bitmap)
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
