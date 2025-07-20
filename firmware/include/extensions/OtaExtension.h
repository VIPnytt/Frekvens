#pragma once

#include "config/constants.h"

#if EXTENSION_OTA

#include <ArduinoOTA.h>

#include "modules/ExtensionModule.h"
#include "services/WebServerService.h"

class OtaExtension : public ExtensionModule
{
private:
    ArduinoOTAClass ArduinoOTA;

    static void onStart();
    static void onEnd();
    static void onError(ota_error_t error);

#ifdef F_INFO
    static void onProgress(size_t index, size_t len);
#endif

#if !defined(OTA_KEY) && !defined(OTA_KEY_HASH)
    static void onUpload(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final);
#endif

public:
    OtaExtension();

    void setup() override;
    void handle() override;
};

extern OtaExtension *Ota;

#endif // EXTENSION_OTA
