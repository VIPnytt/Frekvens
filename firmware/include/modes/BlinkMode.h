#pragma once

#if MODE_BLINK

#include "modules/ModeModule.h"

class BlinkMode final : public ModeModule
{
private:
    bool lit{true};

    unsigned long lastMillis{0UL};

public:
    static constexpr std::string_view name{"Blink"};

    explicit BlinkMode() : ModeModule(name) {};

    void handle() override;
};

#endif // MODE_BLINK
