#pragma once

#if EXTENSION_OTA

#include "modules/ExtensionModule.h"
#include "services/WebServerService.h"

#include <ArduinoOTA.h>

class OtaExtension : public ExtensionModule
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
    OtaExtension();

    void configure() override;
    void begin() override;
    void handle() override;
};

extern OtaExtension *Ota;

#endif // EXTENSION_OTA
