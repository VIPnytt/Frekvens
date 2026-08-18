#if MODE_FLIES

#include "modes/FliesMode.h"

#include "services/DisplayService.h"

/**
 * @brief Updates the display with the current fly positions when an update is pending.
 */
void FliesMode::handle()
{
    if (pending)
    {
        Display.fillFrame(0U);
        for (const std::unordered_map<uint8_t, std::pair<uint8_t, uint8_t>>::value_type &fly : flies)
        {
            Display.setPixel(fly.second.first, fly.second.second, UINT8_MAX);
        }
        pending = false;
    }
}

/**
 * @brief Updates a fly's stored coordinates from a received payload.
 *
 * Invalid payloads are ignored. A valid payload marks the display for updating.
 *
 * @param payload Payload containing the fly ID and its x and y coordinates.
 */
void FliesMode::onReceive(JsonObjectConst payload, std::string_view source)
{
    if (payload["id"].is<uint8_t>() && payload["x"].is<uint8_t>() && payload["y"].is<uint8_t>())
    {
        flies[payload["id"].as<uint8_t>()] =
            std::pair<uint8_t, uint8_t>{payload["x"].as<uint8_t>(), payload["y"].as<uint8_t>()};
        pending = true;
    }
}

#endif // MODE_FLIES
