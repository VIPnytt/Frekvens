#if EXTENSION_SCREENSHOT

#include "extensions/ScreenshotExtension.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/DisplayService.h"

#include <array>

void ScreenshotExtension::handle()
{
    if (pending)
    {
        pending = false;
        transmit();
    }
}

/**
 * @brief Captures the current display frame and transmits it through the screenshot extension.
 */
void ScreenshotExtension::transmit()
{
    std::array<uint8_t, GRID_COLUMNS * GRID_ROWS> frame{};
    Display.getFrame(frame);
    JsonDocument doc; // NOLINT(misc-const-correctness)
    JsonArray _frame{doc["frame"].to<JsonArray>()};
    for (const uint8_t pixel : frame)
    {
        _frame.add(pixel);
    }
    Device.transmit(doc.as<JsonObjectConst>(), name, false);
}

/**
 * @brief Requests capture of a screenshot when the payload action is `"pull"`.
 *
 * @param payload Incoming extension payload.
 */
void ScreenshotExtension::onReceive(JsonObjectConst payload, std::string_view source)
{
    // Action: Pull
    if (payload["action"].is<std::string_view>() && payload["action"].as<std::string_view>() == "pull")
    {
        pending = true;
    }
}

#endif // EXTENSION_SCREENSHOT
