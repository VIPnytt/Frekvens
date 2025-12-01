#pragma once

#if MODE_GLITTER

#include "modules/ModeModule.h"

class GlitterMode : public ModeModule
{
public:
    GlitterMode() : ModeModule("Glitter") {};

    void handle() override;
};

#endif // MODE_GLITTER
