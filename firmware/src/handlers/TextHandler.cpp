#include "config/constants.h"
#include "handlers/TextHandler.h"
#include "services/DisplayService.h"

TextHandler::TextHandler(String text, FontModule *font) : text(text), font(font)
{
    if (text.length())
    {
        {
            uint8_t
                yMax = 0,
                yMin = 0;
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
        spacing = ceil(height / Display.getCellRatio() * 0.1);
        {
            utf8Index = 0;
            uint8_t _width = 0;
            for (uint32_t codepoint; nextCodepoint(codepoint);)
            {
                FontModule::Symbol character = font->getChar(codepoint);
                if (!character.bitmap.empty())
                {
                    _width += calcMsbMax(character) + 1 + character.offsetX + spacing;
                }
                else if (character.offsetX > 0)
                {
                    _width += character.offsetX + spacing;
                }
#ifdef F_DEBUG
                else
                {
                    char utf8buf[5];
                    Serial.printf("%s: missing symbol, %s @ 0x%X %s\n", _name.data(), font->name, codepoint, encodeUtf8(codepoint, utf8buf));
                }
#endif
            }
            if (_width)
            {
                width = _width - spacing;
            }
        }
    }
}

void TextHandler::draw(uint8_t brightness)
{
    draw(max(0, (COLUMNS - width) / 2), (ROWS - height) / 2, brightness);
}

void TextHandler::draw(int16_t x, int8_t y, uint8_t brightness)
{
    utf8Index = 0;
    for (uint32_t codepoint; nextCodepoint(codepoint);)
    {
        FontModule::Symbol character = font->getChar(codepoint);
        if (!character.bitmap.empty())
        {
            uint8_t msbMax = calcMsbMax(character);
            for (uint8_t _x = 0; _x <= msbMax; ++_x)
            {
                for (uint8_t _y = 0; _y < character.bitmap.size(); ++_y)
                {
                    if ((x + character.offsetX + _x) >= 0 && (x + character.offsetX + _x) < COLUMNS && (int16_t)(y + height - character.bitmap.size() - character.offsetY + _y) >= 0 && (int16_t)(y + height - character.bitmap.size() - character.offsetY + _y) < ROWS && (character.bitmap[_y] >> (msbMax - _x)) & 1)
                    {
                        Display.setPixel(x + character.offsetX + _x, y + height - character.bitmap.size() - character.offsetY + _y, brightness);
                    }
                }
            }
            x += msbMax + 1 + character.offsetX + spacing;
        }
        else if (character.offsetX > 0)
        {
            x += character.offsetX + spacing;
        }
    }
}

uint8_t TextHandler::getHeight() const
{
    return height;
}

uint8_t TextHandler::getWidth() const
{
    return width;
}

bool TextHandler::nextCodepoint(uint32_t &out)
{
    if (utf8Index >= text.length())
    {
        return false;
    }
    const uint8_t byte = text[utf8Index++];
    if (byte <= 0x7F)
    {
        out = byte;
        return true;
    }
    uint8_t extraBytes = 0;
    if ((byte & 0xE0) == 0xC0)
    {
        out = byte & 0x1F;
        extraBytes = 1;
    }
    else if ((byte & 0xF0) == 0xE0)
    {
        out = byte & 0x0F;
        extraBytes = 2;
    }
    else if ((byte & 0xF8) == 0xF0)
    {
        out = byte & 0x07;
        extraBytes = 3;
    }
    else
    {
        out = 0xFFFD;
        return true;
    }
    while (extraBytes-- && utf8Index < text.length())
    {
        const uint8_t cont = text[utf8Index++];
        if ((cont & 0xC0) != 0x80)
        {
            out = 0xFFFD;
            break;
        }
        out = (out << 6) | (cont & 0x3F);
    }
    return true;
}

uint8_t TextHandler::calcMsbMax(const FontModule::Symbol &character)
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

const char *TextHandler::encodeUtf8(uint32_t codepoint, char *out)
{
    if (codepoint <= 0x7F)
    {
        out[0] = codepoint;
        out[1] = '\0';
    }
    else if (codepoint <= 0x7FF)
    {
        out[0] = 0xC0 | (codepoint >> 6);
        out[1] = 0x80 | (codepoint & 0x3F);
        out[2] = '\0';
    }
    else if (codepoint <= 0xFFFF)
    {
        out[0] = 0xE0 | (codepoint >> 12);
        out[1] = 0x80 | ((codepoint >> 6) & 0x3F);
        out[2] = 0x80 | (codepoint & 0x3F);
        out[3] = '\0';
    }
    else if (codepoint <= 0x10FFFF)
    {
        out[0] = 0xF0 | (codepoint >> 18);
        out[1] = 0x80 | ((codepoint >> 12) & 0x3F);
        out[2] = 0x80 | ((codepoint >> 6) & 0x3F);
        out[3] = 0x80 | (codepoint & 0x3F);
        out[4] = '\0';
    }
    else
    {
        out[0] = '\0';
    }
    return out;
}
