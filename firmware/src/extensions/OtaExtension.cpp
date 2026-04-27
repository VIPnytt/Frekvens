#if EXTENSION_OTA

#include "extensions/OtaExtension.h"

#include "fonts/LargeFont.h"      // NOLINT(misc-include-cleaner)
#include "handlers/TextHandler.h" // NOLINT(misc-include-cleaner)
#include "services/DisplayService.h"
#include "services/ModesService.h"
#include "services/WebServerService.h"

#include <ESPmDNS.h>
#include <HTTPClient.h>

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
    WebServer.http->on(
        AsyncURIMatcher::exact("/ota"), WebRequestMethod::HTTP_POST, &WebServerService::onEmpty, &onPost);
#endif // OTA_KEY
}

void OtaExtension::handle() { ArduinoOTA.handle(); } // NOLINT(cppcoreguidelines-pro-type-member-init,hicpp-member-init)

void OtaExtension::onStart()
{
    ESP_LOGI("Status", "updating"); // NOLINT(cppcoreguidelines-avoid-do-while)
    Modes.setActive(false);
    const LargeFont font;
    Display.clearFrame();
    TextHandler("U", font).draw();
    Display.flush();
    Display.setPower(true);
}

void OtaExtension::onEnd()
{
    ESP_LOGI("Status", "complete"); // NOLINT(cppcoreguidelines-avoid-do-while)
}

#ifndef OTA_KEY
void OtaExtension::onPost(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data,
                          size_t len, bool final)
{
    if (index == 0)
    {
        onStart();
    }
    if ((index == 0 && !Update.begin(UPDATE_SIZE_UNKNOWN, filename.indexOf("littlefs") >= 0 ? U_LITTLEFS : U_FLASH)) ||
        Update.write(data, len) != len || (final && !Update.end(true)))
    {
        ESP_LOGE("Status", "%s", Update.errorString()); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
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
