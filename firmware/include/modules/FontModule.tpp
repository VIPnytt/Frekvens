#include "modules/FontModule.h"

template <typename T, size_t N> FontModule::Symbol FontModule::toSymbol(const std::array<T, N> &bitmap) const
{
    return {bitmap, 0U, 0};
}

template <typename T, size_t N>
FontModule::Symbol FontModule::toSymbol(const std::array<T, N> &bitmap, int8_t offsetY) const
{
    return {bitmap, 0U, offsetY};
}

template <typename T, size_t N>
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
FontModule::Symbol FontModule::toSymbol(const std::array<T, N> &bitmap, uint8_t offsetX, int8_t offsetY) const
{
    return {bitmap, offsetX, offsetY};
}
