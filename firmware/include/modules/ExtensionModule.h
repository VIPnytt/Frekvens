#pragma once

#include <ArduinoJson.h>

class ExtensionModule
{
protected:
    explicit ExtensionModule(const char *name) : name(name) {};

public:
    virtual ~ExtensionModule() = default;

    ExtensionModule(const ExtensionModule &) = delete;
    ExtensionModule &operator=(const ExtensionModule &) = delete;
    ExtensionModule(ExtensionModule &&) = delete;
    ExtensionModule &operator=(ExtensionModule &&) = delete;

    const char *const name;

    virtual void configure();
    virtual void begin();
    virtual void handle();

    virtual void onReceive(JsonObjectConst payload, const char *source);
    virtual void onTransmit(JsonObjectConst payload, const char *source);
};
