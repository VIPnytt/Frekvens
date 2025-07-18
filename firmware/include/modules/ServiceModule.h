#pragma once

#include <ArduinoJson.h>

class ServiceModule
{
protected:
    ServiceModule(const char *const name) : name(name) {};

public:
    ServiceModule(const ServiceModule &) = delete;
    ServiceModule &operator=(const ServiceModule &) = delete;

    const char *const name;

    virtual void receiverHook(const JsonDocument doc);
};
