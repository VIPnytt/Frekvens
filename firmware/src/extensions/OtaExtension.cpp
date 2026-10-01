#if EXTENSION_OTA

#include "extensions/OtaExtension.h"

#include "fonts/LargeFont.h"      // NOLINT(misc-include-cleaner)
#include "handlers/TextHandler.h" // NOLINT(misc-include-cleaner)
#include "services/DisplayService.h"
#include "services/ModesService.h"

#include <ESPmDNS.h>

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

#endif // EXTENSION_OTA
