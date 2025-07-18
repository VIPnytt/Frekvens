#pragma once

#include <ArduinoJson.h>
#include <vector>

#include "config/constants.h"
#include "modules/ServiceModule.h"

class DeviceService : public ServiceModule
{
private:
    DeviceService() : ServiceModule("Device") {};

    unsigned long lastMillis = 0;

    std::string latest = "";

    JsonDocument transmits;

    void ready();

    void transmit();
    void receiverHook(const JsonDocument doc) override;

public:
    const std::string_view repository = "https://github.com/VIPnytt/Frekvens";

    void init();
    void run();

    void identify();
    void power(bool state);
    void update();

    void transmit(JsonDocument doc, const char *const source, bool retain = true);
    void receive(const JsonDocument doc, const char *const source, const char *const destination);

    const std::vector<const char *> getNames() const;
    const JsonDocument getTransmits() const;

    static DeviceService &getInstance();
};

extern DeviceService &Device;
