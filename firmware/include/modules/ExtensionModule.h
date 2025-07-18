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

    virtual void setup();
    virtual void ready();
    virtual void handle();

    virtual void transmitterHook(const JsonDocument &doc, const char *const source);
    virtual void receiverHook(const JsonDocument doc);
};
