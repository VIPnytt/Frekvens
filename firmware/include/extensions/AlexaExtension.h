#pragma once

#include "config/constants.h"

#if EXTENSION_ALEXA

#include <ESPAsyncWebServer.h>
#include <fauxmoESP.h>

#include "modules/ExtensionModule.h"

class AlexaExtension : public ExtensionModule
{
private:
    fauxmoESP fauxmo = fauxmoESP();

    static void onGetApi(AsyncWebServerRequest *request);
    static void onSetApi(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
    static void onSetState(unsigned char deviceId, const char *deviceName, bool state, unsigned char value);

public:
    AlexaExtension();

    void ready() override;
    void handle() override;

    void transmitterHook(const JsonDocument &doc, const char *const source) override;
};

extern AlexaExtension *Alexa;

#endif // EXTENSION_ALEXA
