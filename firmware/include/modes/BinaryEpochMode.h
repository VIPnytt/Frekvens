#pragma once

#if MODE_BINARYEPOCH

#include "modules/ModeModule.h"

class BinaryEpochMode final : public ModeModule
{
private:
    time_t epoch = 0;

protected:
    ~BinaryEpochMode() = default;

public:
    BinaryEpochMode() : ModeModule("Binary epoch") {};

    void handle() override;
};

#endif // MODE_BINARYEPOCH
