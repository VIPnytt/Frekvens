#if EXTENSION_OTA

#include "extensions/OtaExtension.h"

#include "fonts/LargeFont.h"      // NOLINT(misc-include-cleaner)
#include "handlers/TextHandler.h" // NOLINT(misc-include-cleaner)
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"
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
#if EXTENSION_STATUSLED
    ArduinoOTA.onError(&onError);
#endif // EXTENSION_STATUSLED
}

/**
 * @brief Starts OTA support and registers the unauthenticated upload endpoint when authentication is disabled.
 */
void OtaExtension::begin()
{
    ArduinoOTA.begin();
#ifdef OTA_KEY
    MDNS.enableArduino(3232U, true);
#else
    MDNS.enableArduino(3232U, false);
    WebServer.http->on(
        AsyncURIMatcher::exact("/ota"), WebRequestMethod::HTTP_POST, &WebServerService::onEmpty, &onPost);
#endif // OTA_KEY
}

void OtaExtension::handle() { ArduinoOTA.handle(); }

/**
 * @brief Prepares the device display for an OTA update.
 */
void OtaExtension::onStart()
{
    ESP_LOGI(name.data(), "updating"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    Display.setPower(true);
    Modes.setActive(false);
    const LargeFont font;
    Display.fillFrame(0U);
    TextHandler("U", font).draw();
    Display.flush();
}

/**
 * @brief Logs completion of the OTA update.
 */
void OtaExtension::onEnd()
{
    ESP_LOGI(name.data(), "complete"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
}

#if EXTENSION_STATUSLED
/**
 * @brief Signals an OTA update error through the status LED.
 *
 * @param error OTA error code.
 */
void OtaExtension::onError(ota_error_t error) { Extensions.StatusLed().error(); }
#endif // EXTENSION_STATUSLED

#ifndef OTA_KEY
/**
 * @brief Processes a chunk of an unauthenticated HTTP OTA firmware upload.
 *
 * Selects the LittleFS or flash update target from the filename, writes the
 * upload data, and finalizes the update when the last chunk is received.
 * Sends HTTP 500 when initialization, writing, or finalization fails; sends
 * HTTP 204 after successful completion.
 *
 * @param request HTTP request used to send the upload result.
 * @param filename Uploaded file name used to select the update target.
 * @param index Zero-based offset of the current chunk in the upload.
 * @param data Upload chunk data.
 * @param len Number of bytes in the current chunk.
 * @param final Whether the current chunk completes the upload.
 */
void OtaExtension::onPost(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data,
                          size_t len, bool final)
{
    if (index == 0U)
    {
        onStart();
    }
    if ((index == 0U && !Update.begin(UPDATE_SIZE_UNKNOWN, filename.indexOf("littlefs") >= 0 ? U_LITTLEFS : U_FLASH)) ||
        Update.write(data, len) != len || (final && !Update.end(true)))
    {
        ESP_LOGE(name.data(), "%s", Update.errorString()); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
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
