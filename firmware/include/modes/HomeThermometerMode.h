#pragma once

#if MODE_HOMETHERMOMETER

#include "modules/ModeModule.h"

class HomeThermometerMode final : public ModeModule
{
private:
    static inline int16_t indoor{0};
    static inline int16_t outdoor{0};

    bool pending{false};

    void draw();
    void setTemperature(std::string_view where, int16_t temperature);
    void transmit();

public:
    static constexpr std::string_view name{"Home thermometer"};

    explicit HomeThermometerMode() : ModeModule(name) {};

    void configure() override;
    void begin() override;
    void handle() override;

    void onReceive(JsonObjectConst payload, std::string_view source) override;

#if EXTENSION_HOMEASSISTANT
    void onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) override;
#endif
};

#endif // MODE_HOMETHERMOMETER
