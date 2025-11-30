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

    virtual void configure();
    virtual void begin();
    virtual void handle();
    virtual void end();

    virtual void onReceive(const JsonDocument doc, const char *source);
};
