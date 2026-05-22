#pragma once

#if MODE_BINARYCLOCK

#include "modules/ModeModule.h"

class BinaryClockMode final : public ModeModule
{
private:
    tm local{};

    bool pending{false};

    uint8_t hour{24U};
    uint8_t minute{60U};
    uint8_t second{60U};

    void draw(uint8_t y, uint8_t digit);

public:
    static constexpr std::string_view name{"Binary clock"};

    explicit BinaryClockMode() : ModeModule(name) {};

    void begin() override;
    void handle() override;
};

#endif // MODE_BINARYCLOCK
