#pragma once

#if MODE_GLITTER

#include "modules/ModeModule.h"

class GlitterMode final : public ModeModule
{
protected:
    ~GlitterMode() = default;

public:
    GlitterMode() : ModeModule("Glitter") {};
    GlitterMode(const GlitterMode &) = delete;
    GlitterMode &operator=(const GlitterMode &) = delete;

    void handle() override;
};

#endif // MODE_GLITTER
