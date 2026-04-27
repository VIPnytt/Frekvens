#pragma once

#if MODE_GLITTER

#include "modules/ModeModule.h"

class GlitterMode final : public ModeModule
{
public:
    static constexpr std::string_view name{"Glitter"};

    explicit GlitterMode() : ModeModule(name) {};

    void handle() override;
};

#endif // MODE_GLITTER
