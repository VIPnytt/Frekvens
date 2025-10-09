#pragma once

#include "config/constants.h"

#if MODE_LINES

#include "modules/ModeModule.h"

class LinesMode : public ModeModule
{
protected:
    uint8_t x = 0;

    unsigned long lastMillis = 0;

public:
    LinesMode() : ModeModule("Lines") {};

    void handle() override;
};

#endif // MODE_LINES
