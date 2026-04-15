#pragma once

#if MODE_BINARYEPOCH

#include "modules/ModeModule.h"

class BinaryEpochMode final : public ModeModule
{
private:
    time_t epoch = 0;

public:
    static constexpr std::string_view name{"Binary epoch"};

    explicit BinaryEpochMode() : ModeModule(name) {};

    void handle() override;
};

#endif // MODE_BINARYEPOCH
