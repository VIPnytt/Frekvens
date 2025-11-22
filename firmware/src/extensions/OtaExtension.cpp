#include "config/constants.h"

#if EXTENSION_OTA

#include <HTTPClient.h>

#include "extensions/OtaExtension.h"
#include "fonts/LargeFont.h"
#include "handlers/TextHandler.h"
#include "services/ConnectivityService.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
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

#ifdef OTA_KEY
    ArduinoOTA.setPasswordHash(OTA_KEY);
#else
    WebServer.http->on("/ota", WebRequestMethod::HTTP_POST, [](AsyncWebServerRequest *request) {}, &onUpload);
#endif // OTA_KEY

    ArduinoOTA.begin();
    ArduinoOTA.onStart(&onStart);
    ArduinoOTA.onError(&onError);
    ArduinoOTA.onEnd(&onEnd);
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
    Display.setPower(true, Ota->name);
    timerWrite(Display.timer, 1'000'000 / (1 << 8)); // 1 fps
}

void OtaExtension::onEnd()
{
    ESP_LOGI(Ota->name, "complete");
    Device.setPower(true, Ota->name);
}

void OtaExtension::onError(ota_error_t error)
{
    ESP_LOGE(Ota->name, "%s", Update.errorString());
    Device.setPower(true, Ota->name);
}

#ifndef OTA_KEY
void OtaExtension::onUpload(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if (!index)
    {
        onStart();
    }
    if ((!index && !Update.begin(UPDATE_SIZE_UNKNOWN, filename.indexOf("littlefs") >= 0 ? U_LITTLEFS : U_FLASH)) || Update.write(data, len) != len || (final && !Update.end(true)))
    {
        ESP_LOGE(Ota->name, "%s", Update.errorString());
        request->send(t_http_codes::HTTP_CODE_INTERNAL_SERVER_ERROR);
        Device.setPower(true, Ota->name);
    }
    else if (final)
    {
        request->send(t_http_codes::HTTP_CODE_NO_CONTENT);
        onEnd();
    }
}
#endif // OTA_KEY

#endif // EXTENSION_OTA
