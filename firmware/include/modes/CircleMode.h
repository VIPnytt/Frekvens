#pragma once

#if MODE_CIRCLE

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ModeModule.h"

class CircleMode final : public ModeModule
{
private:
    static constexpr float x = (GRID_COLUMNS - 1) / 2.0F;
    static constexpr float y = (GRID_ROWS - 1) / 2.0F;

    static inline const uint8_t maxRadius{static_cast<uint8_t>(ceilf(
        hypotf(((GRID_COLUMNS - 1) / 2.0F) *
                   (static_cast<float>(2 * PITCH_HORIZONTAL) / static_cast<float>(PITCH_HORIZONTAL + PITCH_VERTICAL)),
               ((GRID_ROWS - 1) / 2.0F) *
                   (static_cast<float>(2 * PITCH_VERTICAL) / static_cast<float>(PITCH_HORIZONTAL + PITCH_VERTICAL)))))};

    bool lit = true;

    uint8_t radius = 0;

    unsigned long lastMillis = 0;

public:
    static constexpr std::string_view name{"Circle"};

    explicit CircleMode() : ModeModule(name) {};

    void handle() override;
};

#endif // MODE_CIRCLE
