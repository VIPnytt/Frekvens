#pragma once

#if MODE_HOMETHERMOMETER

#include "modules/ModeModule.h"

class HomeThermometerMode : public ModeModule
{
private:
    bool pending = false;

    void update();
    void setTemperature(const char *const where, const int16_t temperature);
    void transmit();

public:
    HomeThermometerMode() : ModeModule("Home thermometer") {};

    void configure() override;
    void begin() override;
    void handle() override;
    void onReceive(const JsonDocument doc, const char *const source) override;
};

#endif // MODE_HOMETHERMOMETER
