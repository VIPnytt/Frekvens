#include "modules/FontModule.h"

template <typename T, std::size_t N>
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
FontModule::Symbol FontModule::toSymbol(const std::array<T, N> &bitmap, uint8_t offsetX, int8_t offsetY) const
{
    return {bitmap, offsetX, offsetY};
}
