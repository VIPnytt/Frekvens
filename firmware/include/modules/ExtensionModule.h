#pragma once

#include <ArduinoJson.h>

class ExtensionModule
{
protected:
    ExtensionModule(const char *const name) : name(name) {};

public:
    ExtensionModule(const ExtensionModule &) = delete;
    ExtensionModule &operator=(const ExtensionModule &) = delete;

    const char *const name;

    virtual void configure();
    virtual void begin();
    virtual void handle();

    virtual void onTransmit(const JsonDocument &doc, const char *const source);
    virtual void onReceive(const JsonDocument doc, const char *source);
};
