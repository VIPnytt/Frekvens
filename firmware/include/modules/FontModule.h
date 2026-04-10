#pragma once

#include <span>
#include <string_view>
#include <variant>

class FontModule
{
public:
    virtual ~FontModule() = default;

    FontModule(const FontModule &) = delete;
    FontModule &operator=(const FontModule &) = delete;
    FontModule(FontModule &&) = delete;
    FontModule &operator=(FontModule &&) = delete;

    struct Symbol
    {
        const std::variant<std::span<const uint8_t>, std::span<const uint16_t>> bitmap{};
        uint8_t offsetX{};
        int8_t offsetY{};
    };

    const std::string_view name{};

    [[nodiscard]] virtual Symbol getChar(uint32_t character) const = 0;

protected:
    explicit FontModule(std::string_view name) : name(name) {};

    template <typename T, std::size_t N>
    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    [[nodiscard]] Symbol toSymbol(const std::array<T, N> &bitmap, uint8_t offsetX = 0, int8_t offsetY = 0) const
    {
        return {bitmap, offsetX, offsetY};
    }

    [[nodiscard]] Symbol whitespace(uint8_t offsetX) const;
};
