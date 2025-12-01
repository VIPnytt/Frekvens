#if EXTENSION_OTA

#include <esp_arduino_version.h> // temporary bugfix mitigation due to ledcFade bug in Arduino 3.3.2 and higher
#include <ESPmDNS.h>
#include <HTTPClient.h>

#include "extensions/OtaExtension.h"
#include "fonts/LargeFont.h"
#include "handlers/TextHandler.h"
#include "services/ConnectivityService.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ModesService.h"

#if ESP_ARDUINO_VERSION < ESP_ARDUINO_VERSION_VAL(3, 3, 2)
// Temporary mitigation measure due to ledcFade bug in Arduino 3.3.2 and higher
#warning "Arduino 3.3.1 and lower: OTA may fail to compile or update. Disable OTA if you need this version."
#endif

OtaExtension *Ota = nullptr;

OtaExtension::OtaExtension() : ExtensionModule("OTA")
{
    Ota = this;
}

void OtaExtension::configure()
{
    ArduinoOTA.setHostname(HOSTNAME);
    ArduinoOTA.setMdnsEnabled(false);
#ifdef OTA_KEY
    ArduinoOTA.setPasswordHash(OTA_KEY);
#endif // OTA_KEY
    ArduinoOTA.onStart(&onStart);
    ArduinoOTA.onEnd(&onEnd);
}

void OtaExtension::begin()
{
    ArduinoOTA.begin();
#ifdef OTA_KEY
    MDNS.enableArduino(3232, true);
#else
    MDNS.enableArduino(3232, false);
    WebServer.http->on("/ota", WebRequestMethod::HTTP_POST, [](AsyncWebServerRequest *request) {}, &onPost);
#endif // OTA_KEY
}

void OtaExtension::handle()
{
    ArduinoOTA.handle();
}

void OtaExtension::onStart()
{
    ESP_LOGI(Ota->name, "updating");
    Modes.setActive(false);
    Display.clearFrame();
    TextHandler("U", FontLarge).draw();
    Display.flush();
    Display.setPower(true);
    timerWrite(Display.timer, 1'000'000 / (1 << 8)); // 1 fps
}

void OtaExtension::onEnd()
{
    ESP_LOGI(Ota->name, "complete");
}

#ifndef OTA_KEY
void OtaExtension::onPost(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if (!index)
    {
        onStart();
    }
    if ((!index && !Update.begin(UPDATE_SIZE_UNKNOWN, filename.indexOf("littlefs") >= 0 ? U_LITTLEFS : U_FLASH)) || Update.write(data, len) != len || (final && !Update.end(true)))
    {
        ESP_LOGE(Ota->name, "%s", Update.errorString());
        request->send(t_http_codes::HTTP_CODE_INTERNAL_SERVER_ERROR);
    }
    else if (final)
    {
        request->send(t_http_codes::HTTP_CODE_NO_CONTENT);
        onEnd();
    }
}
#endif // OTA_KEY

#endif // EXTENSION_OTA
