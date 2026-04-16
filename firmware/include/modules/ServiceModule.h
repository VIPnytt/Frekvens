#pragma once

#include <ArduinoJson.h> // NOLINT(misc-include-cleaner)
#include <string_view>

class ServiceModule
{
protected:
    explicit ServiceModule(std::string_view name) : name(name) {};

public:
    virtual ~ServiceModule() = default;

    ServiceModule(const ServiceModule &) = delete;
    ServiceModule &operator=(const ServiceModule &) = delete;
    ServiceModule(ServiceModule &&) = delete;
    ServiceModule &operator=(ServiceModule &&) = delete;

    const std::string_view name{};

    virtual void onReceive(JsonObjectConst payload, std::string_view source);

#if EXTENSION_HOMEASSISTANT
    virtual void onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique);
#endif
};
