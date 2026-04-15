#pragma once

#if MODE_BRIGHT

#include "modules/ModeModule.h"

class BrightMode final : public ModeModule
{
public:
    static constexpr std::string_view name{"Bright"};

    explicit BrightMode() : ModeModule(name) {};

    void begin() override;
};

#endif // MODE_BRIGHT
