#pragma once

#include "modules/ServiceModule.h"

#include <ArduinoJson.h> // NOLINT(misc-include-cleaner)

class DeviceService final : public ServiceModule
{
private:
    explicit DeviceService() : ServiceModule("Device") {};

    bool operational = false;

    unsigned long lastMillis = 0;

    JsonDocument transmits;

    void transmit();

    void onReceive(JsonObjectConst payload, std::string_view source) override;

public:
    TaskHandle_t taskHandle = nullptr;

    void begin();
    void handle();

    void setPower(bool power);
    void restore();

    void transmit(JsonObjectConst payload, std::string_view source, bool retain = true);
    void receive(JsonObjectConst payload, std::string_view source, std::string_view destination) const;

    [[nodiscard]] JsonObjectConst getTransmits() const;

#if EXTENSION_HOMEASSISTANT
    void onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) override;
#endif

    static DeviceService &getInstance();
};

extern DeviceService &Device; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
