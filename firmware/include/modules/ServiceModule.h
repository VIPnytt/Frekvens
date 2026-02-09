#pragma once

#include <ArduinoJson.h>

class ServiceModule
{
protected:
    explicit ServiceModule(const char *name) : name(name) {};

public:
    const char *const name;

    virtual void onReceive(JsonObjectConst payload, const char *source);
};
