#include "handlers/TextHandler.h"

#include "config/constants.h"
#include "services/DisplayService.h"

TextHandler::TextHandler(std::string text, FontModule *font) : text(text), font(font)
{
    if (text.length())
    {
        {
            uint8_t yMax = 0;
            uint8_t yMin = 0;
            for (uint32_t codepoint; nextCodepoint(codepoint);)
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
        tracking = static_cast<uint8_t>(ceilf(height / Display.getRatio() / 10.0f));
        {
            i = 0;
            uint8_t _width = 0;
            for (uint32_t codepoint; nextCodepoint(codepoint);)
            {
                FontModule::Symbol character = font->getChar(codepoint);
                if (!character.bitmap.empty())
                {
                    _width += calcMsbMax(character) + 1 + character.offsetX + tracking;
                }
                else if (character.offsetX > 0)
                {
                    _width += character.offsetX + tracking;
                }
                else
                {
                    char buffer[5];
                    ESP_LOGV(font->name, "missing symbol 0x%X %s", codepoint, encode(codepoint, buffer));
                }
            }
            if (_width > tracking)
            {
                width = _width - tracking;
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
    for (uint32_t codepoint; nextCodepoint(codepoint);)
    {
        FontModule::Symbol character = font->getChar(codepoint);
        const uint8_t _height = character.bitmap.size();
        if (_height != 0)
        {
            const uint8_t msbMax = calcMsbMax(character);
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
    const uint8_t byte = text[i++];
    if (byte <= 0x7F)
    {
        buffer = byte;
        return true;
    }
    uint8_t bytes = 0;
    if ((byte & 0xE0) == 0xC0)
    {
        buffer = byte & 0x1F;
        bytes = 1;
    }
    else if ((byte & 0xF0) == 0xE0)
    {
        buffer = byte & 0x0F;
        bytes = 2;
    }
    else if ((byte & 0xF8) == 0xF0)
    {
        buffer = byte & 0x07;
        bytes = 3;
    }
    else
    {
        buffer = 0xFFFD;
        return true;
    }
    while (bytes-- && i < text.length())
    {
        const uint8_t cont = text[i++];
        if ((cont & 0xC0) != 0x80)
        {
            buffer = 0xFFFD;
            break;
        }
        buffer = (buffer << 6) | (cont & 0x3F);
    }
    return true;
}

uint8_t TextHandler::calcMsbMax(const FontModule::Symbol &character) const
{
    uint8_t msbMax = 0;
    for (uint8_t bitset : character.bitmap)
    {
        uint8_t msb = 0;
        while (bitset >>= 1)
        {
            ++msb;
        }
        if (msb > msbMax)
        {
            msbMax = msb;
        }
    }
    return msbMax;
}

const char *TextHandler::encode(uint32_t codepoint, char *buffer)
{
    if (codepoint <= 0x7F)
    {
        buffer[0] = codepoint;
        buffer[1] = '\0';
    }
    else if (codepoint <= 0x7FF)
    {
        buffer[0] = 0xC0 | (codepoint >> 6);
        buffer[1] = 0x80 | (codepoint & 0x3F);
        buffer[2] = '\0';
    }
    else if (codepoint <= 0xFFFF)
    {
        buffer[0] = 0xE0 | (codepoint >> 12);
        buffer[1] = 0x80 | ((codepoint >> 6) & 0x3F);
        buffer[2] = 0x80 | (codepoint & 0x3F);
        buffer[3] = '\0';
    }
    else if (codepoint <= 0x10FFFF)
    {
        buffer[0] = 0xF0 | (codepoint >> 18);
        buffer[1] = 0x80 | ((codepoint >> 12) & 0x3F);
        buffer[2] = 0x80 | ((codepoint >> 6) & 0x3F);
        buffer[3] = 0x80 | (codepoint & 0x3F);
        buffer[4] = '\0';
    }
    else
    {
        buffer[0] = '\0';
    }
    return buffer;
}
