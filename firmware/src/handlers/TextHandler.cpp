#include "handlers/TextHandler.h"

#include "config/constants.h"        // NOLINT(misc-include-cleaner)
#include "services/DisplayService.h" // NOLINT(misc-include-cleaner)

TextHandler::TextHandler(std::string text, const FontModule &font) : text(text), font(&font)
{
    if (text.length())
    {
        {
            size_t index{0U}; // NOLINT(misc-const-correctness)
            int8_t yMax{0};   // NOLINT(misc-const-correctness)
            int8_t yMin{0};
            for (char32_t codepoint{0U}; nextCodepoint(index, codepoint);) // NOLINT(misc-const-correctness)
            {
                const FontModule::Symbol character{font.getChar(codepoint)};
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
            width = 0U;
            size_t index{0U};                                              // NOLINT(misc-const-correctness)
            for (char32_t codepoint{0U}; nextCodepoint(index, codepoint);) // NOLINT(misc-const-correctness)
            {
                const FontModule::Symbol character{font.getChar(codepoint)};
                std::visit(
                    [&](auto &&bitmap)
                    {
                        if (!bitmap.empty())
                        {
                            width += calcMsbMax(bitmap) + 1U + character.offsetX + tracking;
                        }
                        else if (character.offsetX != 0U)
                        {
                            width += character.offsetX + tracking;
                        }
                        else
                        {
                            ESP_LOGV("Font", "missing symbol 0x%X %s", codepoint, encode(codepoint).data());
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
    draw(max(0U, (GRID_COLUMNS - width) / 2U), (GRID_ROWS - height) / 2U, brightness);
}

void TextHandler::draw(int16_t x, int8_t y, uint8_t brightness) const
{
    size_t index{0U};                                              // NOLINT(misc-const-correctness)
    for (char32_t codepoint{0U}; nextCodepoint(index, codepoint);) // NOLINT(misc-const-correctness)
    {
        const FontModule::Symbol character{font->getChar(codepoint)};
        std::visit(
            [&](auto &&bitmap)
            {
                const uint8_t _height{static_cast<uint8_t>(bitmap.size())};
                if (_height != 0U)
                {
                    const uint8_t msbMax{calcMsbMax(bitmap)};
                    for (uint16_t _x{0U}; _x <= msbMax; ++_x)
                    {
                        for (uint8_t _y{0U}; _y < _height; ++_y)
                        {
                            const int pixelX{static_cast<int>(x) + static_cast<int>(character.offsetX) +
                                             static_cast<int>(_x)};
                            const int pixelY{static_cast<int>(y) + static_cast<int>(height) -
                                             static_cast<int>(_height) - static_cast<int>(character.offsetY) +
                                             static_cast<int>(_y)};
                            if (pixelX >= 0 && pixelX < GRID_COLUMNS && pixelY >= 0 && pixelY < GRID_ROWS &&
                                (((bitmap[_y] >> (msbMax - _x)) & 0b1U) != 0U))
                            {
                                Display.setPixel(
                                    static_cast<uint8_t>(pixelX), static_cast<uint8_t>(pixelY), brightness);
                            }
                        }
                    }
                    x += msbMax + 1 + character.offsetX + tracking;
                }
                else if (character.offsetX != 0U)
                {
                    x += character.offsetX + tracking;
                }
            },
            character.bitmap);
    }
}

uint8_t TextHandler::getHeight() const { return height; }

uint8_t TextHandler::getWidth() const { return width; }

bool TextHandler::nextCodepoint(size_t &index, char32_t &buffer) const
{
    if (index >= text.length())
    {
        return false;
    }
    const uint8_t byte{static_cast<uint8_t>(text[index])}; // NOLINT(cppcoreguidelines-init-variables)
    index++;
    if (byte < (0b1U << 7U))
    {
        buffer = byte;
        return true;
    }
    uint8_t bytes{0U};
    if ((byte & (0b111U << 5U)) == 0b11U << 6U)
    {
        buffer = byte & 0b1'1111U;
        bytes = 1U;
    }
    else if ((byte & (0b1111U << 4U)) == 0b111U << 5U)
    {
        buffer = byte & 0b1111U;
        bytes = 2U;
    }
    else if ((byte & (0b1'1111U << 3U)) == 0b1111U << 4U)
    {
        buffer = byte & 0b111U;
        bytes = 3U;
    }
    else
    {
        buffer = 0b1111'1111'1111'1101U;
        return true;
    }
    while (bytes-- && index < text.length())
    {
        const uint8_t _byte{static_cast<uint8_t>(text[index])}; // NOLINT(cppcoreguidelines-init-variables)
        index++;
        if ((_byte & (0b11U << 6U)) != 0b1U << 7U)
        {
            buffer = 0b1111'1111'1111'1101U;
            break;
        }
        buffer = (buffer << 6U) | (_byte & 0b11'1111U);
    }
    return true;
}

template <typename T>
    requires std::is_unsigned_v<T>
uint8_t TextHandler::calcMsbMax(std::span<const T> bitmap) const
{
    uint8_t msbMax{0U}; // NOLINT(misc-const-correctness)
    for (const T bitset : bitmap)
    {
        if (bitset != 0U)
        {
            msbMax = max(msbMax, static_cast<uint8_t>(std::bit_width(bitset) - 1));
        }
    }
    return msbMax;
}

std::array<char, 5U> TextHandler::encode(char32_t codepoint)
{
    std::array<char, 5U> out{};
    if (codepoint < (0b1U << 7U))
    {
        out.at(0U) = static_cast<char>(codepoint);
    }
    else if (codepoint < (0b1U << 11U))
    {
        out.at(0U) = static_cast<char>((0b11U << 6U) | (codepoint >> (0b11U << 1U)));
        out.at(1U) = static_cast<char>((0b1U << 7U) | (codepoint & 0b11'1111U));
    }
    else if (codepoint < (0b1U << 16U))
    {
        out.at(0U) = static_cast<char>((0b111U << 5U) | (codepoint >> (0b11U << 2U)));
        out.at(1U) = static_cast<char>((0b1U << 7U) | ((codepoint >> (0b11U << 1U)) & 0b11'1111U));
        out.at(2U) = static_cast<char>((0b1U << 7U) | (codepoint & 0b11'1111U));
    }
    else if (codepoint < (0b10001U << 16U))
    {
        out.at(0U) = static_cast<char>((0b1U << 7U) | (codepoint >> (0b1'001U << 1U)));
        out.at(1U) = static_cast<char>((0b1U << 7U) | ((codepoint >> (0b11U << 2U)) & 0b11'1111U));
        out.at(2U) = static_cast<char>((0b1U << 7U) | ((codepoint >> (0b11U << 1U)) & 0b11'1111U));
        out.at(3U) = static_cast<char>((0b1U << 7U) | (codepoint & 0b11'1111U));
    }
    return out;
}
