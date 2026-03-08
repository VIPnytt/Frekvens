#pragma once

#include <ArduinoJson.h> // NOLINT(misc-include-cleaner)

class ServiceModule
{
protected:
    explicit ServiceModule(const char *name) : name(name) {};

public:
    virtual ~ServiceModule() = default;

    ServiceModule(const ServiceModule &) = delete;
    ServiceModule &operator=(const ServiceModule &) = delete;
    ServiceModule(ServiceModule &&) = delete;
    ServiceModule &operator=(ServiceModule &&) = delete;

    const char *const name;

    virtual void onReceive(JsonObjectConst payload, const char *source);
};
