#pragma once

#include "modules/HandlerModule.h" // NOLINT(misc-include-cleaner)

#include <span>
#include <string_view>

template <typename T>
    requires std::is_unsigned_v<T>
class BitmapHandler;

template <typename T> BitmapHandler(std::span<const T>) -> BitmapHandler<T>;

template <typename T, std::size_t N> BitmapHandler(const std::array<T, N> &) -> BitmapHandler<std::remove_cv_t<T>>;

template <typename T, std::size_t N> BitmapHandler(const T (&)[N]) -> BitmapHandler<std::remove_cv_t<T>>;

template <typename T>
    requires std::is_unsigned_v<T>
class BitmapHandler final : public HandlerModule
{
private:
    static constexpr std::string_view _name = "BitmapHandler";

    std::span<const T> bitmap{};

    uint8_t height = 0;
    uint8_t width = 0;

public:
    explicit BitmapHandler(std::span<const T> bitmap);

    void draw(uint8_t brightness = UINT8_MAX) const;
    void draw(uint8_t x, uint8_t y, uint8_t brightness = UINT8_MAX) const;
    [[nodiscard]] uint8_t getHeight() const;
    [[nodiscard]] uint8_t getWidth() const;
};

#include "handlers/BitmapHandler.tpp"
