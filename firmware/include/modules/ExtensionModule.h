#pragma once

#include <ArduinoJson.h>

class ExtensionModule
{
protected:
    explicit ExtensionModule(const char *const name) : name(name) {};

public:
    const char *const name;

    virtual void configure();
    virtual void begin();
    virtual void handle();

    virtual void onReceive(const JsonDocument &doc, const char *source);
    virtual void onTransmit(const JsonDocument &doc, const char *source);
};
