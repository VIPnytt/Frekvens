#pragma once

#if MODE_LINES

#include "modules/ModeModule.h"

class LinesMode final : public ModeModule
{
private:
    uint8_t x = 0;

    unsigned long lastMillis = 0;

public:
    static constexpr std::string_view name{"Lines"};

    explicit LinesMode() : ModeModule(name) {};

    void handle() override;
};

#endif // MODE_LINES
