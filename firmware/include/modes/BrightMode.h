#pragma once

#include "modules/ModeModule.h"

class BrightMode : public ModeModule
{
public:
    BrightMode() : ModeModule("Bright") {};

    void wake() override;
};
