#pragma once

#include "modules/ModeModule.h"

class GlitterMode : public ModeModule
{
public:
    GlitterMode() : ModeModule("Glitter") {};

    void handle() override;
};
