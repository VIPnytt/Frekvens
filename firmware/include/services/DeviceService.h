#pragma once

#include "config/constants.h"
#include "modules/ServiceModule.h"

#include <vector>

class DeviceService : public ServiceModule
{
private:
    DeviceService() : ServiceModule("Device") {};

    bool operational = false;

    unsigned long lastMillis = 0;

    JsonDocument transmits;

    void transmit();

    void onReceive(const JsonDocument doc, const char *const source) override;

public:
    TaskHandle_t taskHandle = nullptr;

    void begin();
    void handle();

    void setPower(bool power);
    void restore();

    void transmit(JsonDocument doc, const char *const source, bool retain = true);
    void receive(const JsonDocument doc, const char *const source, const char *const destination);

    const JsonDocument getTransmits() const;

    static DeviceService &getInstance();
};

extern DeviceService &Device;
