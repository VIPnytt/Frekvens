#pragma once

#include "config/constants.h"

#if MODE_HOMETHERMOMETER

#include "modules/ModeModule.h"

class HomeThermometerMode : public ModeModule
{
private:
    bool pending = false;

    void update();
    void set(const char *const where, const int16_t temperature);
    void transmit();

public:
    HomeThermometerMode() : ModeModule("Home thermometer") {};

    void setup() override;
    void wake() override;
    void handle() override;
    void receiverHook(const JsonDocument doc) override;
};

#endif // MODE_HOMETHERMOMETER
