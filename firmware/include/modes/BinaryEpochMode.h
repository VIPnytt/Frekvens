#pragma once

#include "config/constants.h"

#if MODE_BINARYEPOCH

#include "modules/ModeModule.h"

class BinaryEpochMode : public ModeModule
{
private:
    time_t epoch;

public:
    BinaryEpochMode() : ModeModule("Binary epoch") {};

    void handle() override;
};

#endif // MODE_BINARYEPOCH
