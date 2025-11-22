#pragma once

#include <vector>

#include "config/constants.h"
#include "modules/ServiceModule.h"

class DeviceService : public ServiceModule
{
private:
    DeviceService() : ServiceModule("Device") {};

    bool operational = false;

    unsigned long lastMillis = 0;

    JsonDocument transmits;

    void ready();

    void transmit();
    void receiverHook(const JsonDocument doc, const char *const source) override;

public:
    TaskHandle_t taskHandle = nullptr;

    void init();
    void run();

    void setPower(bool power, const char *const source);
    void restore();

    void transmit(JsonDocument doc, const char *const source, bool retain = true);
    void receive(const JsonDocument doc, const char *const source, const char *const destination);

    const std::vector<const char *> getNames() const;
    const JsonDocument getTransmits() const;

    static DeviceService &getInstance();
};

extern DeviceService &Device;
