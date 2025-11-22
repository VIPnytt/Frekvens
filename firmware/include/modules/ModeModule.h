#pragma once

#include <ArduinoJson.h>

class ModeModule
{
protected:
    ModeModule(const char *const name) : name(name) {};

public:
    ModeModule(const ModeModule &) = delete;
    ModeModule &operator=(const ModeModule &) = delete;

    const char *const name;

    virtual void setup();
    virtual void wake();
    virtual void handle();
    virtual void sleep();

    virtual void receiverHook(const JsonDocument doc, const char *source);
};
