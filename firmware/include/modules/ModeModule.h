#pragma once

#include <ArduinoJson.h> // NOLINT(misc-include-cleaner)
#include <string_view>

class ModeModule
{
protected:
    explicit ModeModule(std::string_view name) : name(name) {};

public:
    virtual ~ModeModule() = default;

    ModeModule(const ModeModule &) = delete;
    ModeModule &operator=(const ModeModule &) = delete;
    ModeModule(ModeModule &&) = delete;
    ModeModule &operator=(ModeModule &&) = delete;

    std::string_view const name;

    virtual void configure();
    virtual void begin();
    virtual void handle();
    virtual void end();

    virtual void onReceive(JsonObjectConst payload, std::string_view source);
};
