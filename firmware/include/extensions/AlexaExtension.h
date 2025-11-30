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

    static void onGet(AsyncWebServerRequest *request);
    static void onSet(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
    static void onSetState(unsigned char deviceId, const char *deviceName, bool state, unsigned char value);

public:
    AlexaExtension();

    void begin() override;
    void handle() override;

    void onTransmit(const JsonDocument &doc, const char *const source) override;
};

extern AlexaExtension *Alexa;

#endif // EXTENSION_ALEXA
