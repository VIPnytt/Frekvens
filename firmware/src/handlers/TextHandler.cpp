#include "config/constants.h"
#include "handlers/TextHandler.h"
#include "services/DisplayService.h"

TextHandler::TextHandler(String text, FontModule *font) : text(text), font(font)
{
    if (text.length())
    {
        {
            uint8_t
                maxY = 0,
                minY = 0,
                multiplier = 0;
            for (uint8_t charIndex = 0; charIndex < text.length(); ++charIndex)
            {
                if (text[charIndex] >= 192 && text[charIndex] <= UINT8_MAX)
                {
                    multiplier = text[charIndex];
                    continue;
                }
                FontModule::Symbol character = font->getChar((1U << 8) * multiplier + text[charIndex]);
                if (!character.bitmap.empty() && character.bitmap.size() + character.offsetY > maxY)
                {
                    maxY = character.bitmap.size() + character.offsetY;
                }
                if (character.offsetY < minY)
                {
                    minY = character.offsetY;
                }
                multiplier = 0;
            }
            height = maxY - minY;
        }
        spacing = ceil(height / Display.getCellRatio() * 0.1);
        {
            uint8_t
                _width = 0,
                multiplier = 0;
            for (uint8_t charIndex = 0; charIndex < text.length(); ++charIndex)
            {
                if (text[charIndex] >= 192 && text[charIndex] <= UINT8_MAX)
                {
                    multiplier = text[charIndex];
                    continue;
                }
                FontModule::Symbol character = font->getChar((1U << 8) * multiplier + text[charIndex]);
                if (!character.bitmap.empty())
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
                    _width += msbMax + 1 + character.offsetX + spacing;
                }
                else if (character.offsetX > 0)
                {
                    _width += character.offsetX + spacing;
                }
#ifdef F_DEBUG
                else
                {
                    Serial.printf("%s: missing symbol, %s @ 0x%X %s\n", name, font->name, (1U << 8) * multiplier + text[charIndex], text[charIndex]);
                }
#endif
                multiplier = 0;
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
    uint8_t multiplier = 0;
    for (uint8_t charIndex = 0; charIndex < text.length(); ++charIndex)
    {
        if (text[charIndex] >= 192 && text[charIndex] <= UINT8_MAX)
        {
            multiplier = text[charIndex];
            continue;
        }
        FontModule::Symbol character = font->getChar((1U << 8) * multiplier + text[charIndex]);
        if (!character.bitmap.empty())
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
            for (uint8_t _x = 0; _x <= msbMax; ++_x)
            {
                for (uint8_t _y = 0; _y < character.bitmap.size(); ++_y)
                {
                    if (x + character.offsetX + _x >= 0 && x + character.offsetX + _x < COLUMNS && y + height - character.bitmap.size() - character.offsetY + _y >= 0 && y + height - character.bitmap.size() - character.offsetY + _y < ROWS && character.bitmap[_y] >> msbMax - _x & 1)
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
        multiplier = 0;
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
