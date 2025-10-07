#include "config/constants.h"

#if EXTENSION_OTA

#include <HTTPClient.h>

#include "extensions/BuildExtension.h"
#include "extensions/OtaExtension.h"
#include "fonts/LargeFont.h"
#include "handlers/TextHandler.h"
#include "services/ConnectivityService.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"
#include "services/ModesService.h"

OtaExtension *Ota = nullptr;

OtaExtension::OtaExtension() : ExtensionModule("OTA")
{
    Ota = this;
}

void OtaExtension::setup()
{
    ArduinoOTA.setHostname(HOSTNAME);
    ArduinoOTA.setMdnsEnabled(false);

#ifdef OTA_KEY_HASH
    ArduinoOTA.setPasswordHash(OTA_KEY_HASH);
#elif defined(OTA_KEY)
    ArduinoOTA.setPassword(OTA_KEY);
#else
    WebServer.http->on("/api/ota", WebRequestMethod::HTTP_POST, [](AsyncWebServerRequest *request) {}, &onUpload);
#endif // OTA_KEY_HASH

#ifdef F_INFO
    Update.onProgress(&onProgress);
#endif

    ArduinoOTA.begin();
    ArduinoOTA.onStart(&onStart);
    ArduinoOTA.onError(&onError);
    ArduinoOTA.onEnd(&onEnd);

    JsonDocument doc;
    doc["platformio.ini"]["upload_protocol"] = "espota";
    doc["platformio.ini"]["upload_port"] = Connectivity.domain;
#if defined(OTA_KEY) || defined(OTA_KEY_HASH)
    doc["platformio.ini"]["upload_flags"] = "--auth=REDACTED";
#endif // defined(OTA_KEY_HASH) || defined(OTA_KEY)

#if EXTENSION_BUILD
    (*Build->config)[Config::pio]["upload_protocol"] = "espota";
#ifdef OTA_KEY
    (*Build->config)[Config::env][__STRING(OTA_KEY)] = "REDACTED";
#elif defined(OTA_KEY_HASH)
    (*Build->config)[Config::h][__STRING(OTA_KEY_HASH)] = "REDACTED";
#endif // OTA_KEY
#endif // EXTENSION_BUILD

    Device.transmit(doc, name);
}

void OtaExtension::handle()
{
    ArduinoOTA.handle();
}

void OtaExtension::onStart()
{
    Modes.set(false, Ota->name);
#ifdef F_INFO
    Serial.printf("%s: updating\n", Ota->name);
#endif
    Display.clear();
    TextHandler("U", FontLarge).draw();
    Display.flush();
    Display.setPower(true);
    timerAlarmWrite(Display.timer, 1000000U / (1U << 8), true); // 1 fps
}

void OtaExtension::onEnd()
{
#ifdef F_INFO
    Serial.printf("%s: complete\n", Ota->name);
#endif
    Device.power(true);
}

void OtaExtension::onError(ota_error_t error)
{
#ifdef F_INFO
    Serial.printf("%s: %s\n", Ota->name, Update.errorString());
#endif
    Device.power(true);
}

#ifdef F_INFO
void OtaExtension::onProgress(size_t index, size_t len)
{
    Serial.printf("%s: writing @ 0x%X\n", Ota->name, index);
}
#endif

#if !defined(OTA_KEY) && !defined(OTA_KEY_HASH)
void OtaExtension::onUpload(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if (!index)
    {
        onStart();
    }
    if ((!index && !Update.begin(UPDATE_SIZE_UNKNOWN, filename.indexOf("spiffs") >= 0 ? U_SPIFFS : U_FLASH)) || Update.write(data, len) != len || (final && !Update.end(true)))
    {
#ifdef F_INFO
        Serial.printf("%s: %s\n", Ota->name, Update.errorString());
#endif
        request->send(t_http_codes::HTTP_CODE_INTERNAL_SERVER_ERROR);
        Device.power(true);
    }
    else if (final)
    {
        request->send(t_http_codes::HTTP_CODE_NO_CONTENT);
        onEnd();
    }
}
#endif // !defined(OTA_KEY) && !defined(OTA_KEY_HASH)

#endif // EXTENSION_OTA
