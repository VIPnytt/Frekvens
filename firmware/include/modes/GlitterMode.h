#pragma once

#if MODE_GLITTER

#include "modules/ModeModule.h"

class GlitterMode final : public ModeModule
{
protected:
    ~GlitterMode() = default;

public:
    GlitterMode() : ModeModule("Glitter") {};

    void handle() override;
};

#endif // MODE_GLITTER
