#pragma once

#if EXTENSION_OTA

#include "modules/ExtensionModule.h"

#include <ArduinoOTA.h>
#include <ESPAsyncWebServer.h>

class OtaExtension final : public ExtensionModule
{
private:
    static constexpr std::string_view name{"OTA"};

    ArduinoOTAClass ArduinoOTA;

    static void onStart();
    static void onEnd();

#ifndef OTA_KEY
    static void onPost(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len,
                       bool final);
#endif

public:
    explicit OtaExtension() : ExtensionModule(name) {};

    void configure() override;
    void begin() override;
    void handle() override;
};

#endif // EXTENSION_OTA
