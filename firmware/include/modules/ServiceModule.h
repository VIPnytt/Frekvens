#pragma once

#include <ArduinoJson.h>

class ServiceModule
{
protected:
    explicit ServiceModule(const char *const name) : name(name) {};

public:
    const char *const name;

    virtual void onReceive(const JsonDocument &doc, const char *source);
};
