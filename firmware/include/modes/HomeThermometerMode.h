#pragma once

#if MODE_HOMETHERMOMETER

#include "modules/ModeModule.h"

class HomeThermometerMode final : public ModeModule
{
private:
    bool pending = false;

    void update();
    void setTemperature(const char *where, int16_t temperature);
    void transmit();

public:
    explicit HomeThermometerMode() : ModeModule("Home thermometer") {};

    void configure() override;
    void begin() override;
    void handle() override;
    void onReceive(JsonObjectConst payload, const char *source) override;
};

#endif // MODE_HOMETHERMOMETER
