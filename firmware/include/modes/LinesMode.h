#pragma once

#if MODE_LINES

#include "modules/ModeModule.h"

class LinesMode final : public ModeModule
{
private:
    uint8_t x{0U};

    unsigned long lastMillis{0UL};

public:
    static constexpr std::string_view name{"Lines"};

    explicit LinesMode() : ModeModule(name) {};

    void handle() override;
};

#endif // MODE_LINES
