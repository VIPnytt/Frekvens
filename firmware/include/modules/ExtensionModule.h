#pragma once

#include <ArduinoJson.h> // NOLINT(misc-include-cleaner)
#include <string_view>

class ExtensionModule
{
protected:
    explicit ExtensionModule(std::string_view name) : name(name) {};

public:
    virtual ~ExtensionModule() = default;

    ExtensionModule(const ExtensionModule &) = delete;
    ExtensionModule &operator=(const ExtensionModule &) = delete;
    ExtensionModule(ExtensionModule &&) = delete;
    ExtensionModule &operator=(ExtensionModule &&) = delete;

    const std::string_view name{};

    virtual void configure();
    virtual void begin();
    virtual void handle();

    virtual void onReceive(JsonObjectConst payload, std::string_view source);
    virtual void onTransmit(JsonObjectConst payload, std::string_view source);

#if EXTENSION_HOMEASSISTANT
    virtual void onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique);
#endif
};
