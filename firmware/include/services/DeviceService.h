#pragma once

#include "config/constants.h"
#include "modules/ServiceModule.h"

#include <vector>

class DeviceService final : public ServiceModule
{
private:
    explicit DeviceService() : ServiceModule("Device") {};

    bool operational = false;

    unsigned long lastMillis = 0;

    JsonDocument transmits;

    void transmit();

    void onReceive(JsonObjectConst payload, const char *source) override;

public:
    TaskHandle_t taskHandle = nullptr;

    void begin();
    void handle();

    void setPower(bool power);
    void restore();

    void transmit(JsonObjectConst payload, const char *source, bool retain = true);
    void receive(JsonObjectConst payload, const char *source, const char *destination) const;

    [[nodiscard]] JsonObjectConst getTransmits() const;

    static DeviceService &getInstance();
};

extern DeviceService &Device;
