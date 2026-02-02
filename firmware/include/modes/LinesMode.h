#pragma once

#if MODE_LINES

#include "modules/ModeModule.h"

class LinesMode final : public ModeModule
{
private:
    uint8_t x = 0;

    unsigned long lastMillis = 0;

protected:
    ~LinesMode() = default;

public:
    LinesMode() : ModeModule("Lines") {};
    LinesMode(const LinesMode &) = delete;
    LinesMode &operator=(const LinesMode &) = delete;

    void handle() override;
};

#endif // MODE_LINES
