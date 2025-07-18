#pragma once

#include "modules/ModeModule.h"

class BinaryEpochMode : public ModeModule
{
private:
    time_t epoch;

public:
    BinaryEpochMode() : ModeModule("Binary epoch") {};

    void handle() override;
};
