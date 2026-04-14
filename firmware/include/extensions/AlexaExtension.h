#pragma once

#if EXTENSION_ALEXA

#include "modules/ExtensionModule.h"

#include <ESPAsyncWebServer.h>
#include <fauxmoESP.h>

class AlexaExtension final : public ExtensionModule
{
private:
    static constexpr std::string_view name{"Alexa"};

    static inline fauxmoESP fauxmo = fauxmoESP();

    static void onGet(AsyncWebServerRequest *request);
    static void onSet(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
    static void onSetState(unsigned char deviceId, const char *deviceName, bool state, unsigned char value);

public:
    explicit AlexaExtension() : ExtensionModule(name) {};

    void begin() override;
    void handle() override;

    void onTransmit(JsonObjectConst payload, std::string_view source) override;
};

#endif // EXTENSION_ALEXA
