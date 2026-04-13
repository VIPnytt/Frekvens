#pragma once

#include <ArduinoJson.h> // NOLINT(misc-include-cleaner)

class ModeModule
{
protected:
    explicit ModeModule(const char *name) : name(name) {};

public:
    virtual ~ModeModule() = default;

    ModeModule(const ModeModule &) = delete;
    ModeModule &operator=(const ModeModule &) = delete;
    ModeModule(ModeModule &&) = delete;
    ModeModule &operator=(ModeModule &&) = delete;

    const char *const name;

    virtual void configure();
    virtual void begin();
    virtual void handle();
    virtual void end();

    virtual void onReceive(JsonObjectConst payload, std::string_view source);
};
