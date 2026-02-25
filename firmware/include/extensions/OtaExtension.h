#pragma once

#if EXTENSION_OTA

#include "modules/ExtensionModule.h"

#include <ArduinoOTA.h>
#include <ESPAsyncWebServer.h>

class OtaExtension final : public ExtensionModule
{
private:
    ArduinoOTAClass ArduinoOTA;

    static void onStart();
    static void onEnd();

#ifndef OTA_KEY
    static void onPost(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len,
                       bool final);
#endif

public:
    explicit OtaExtension();

    void configure() override;
    void begin() override;
    void handle() override;
};

extern OtaExtension *Ota; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

#endif // EXTENSION_OTA
