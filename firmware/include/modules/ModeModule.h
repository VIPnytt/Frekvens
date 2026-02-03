#pragma once

#include <ArduinoJson.h>

class ModeModule
{
protected:
    explicit ModeModule(const char *const name) : name(name) {};

public:
    const char *const name;

    virtual void configure();
    virtual void begin();
    virtual void handle();
    virtual void end();

    virtual void onReceive(const JsonDocument &doc, const char *source);
};
