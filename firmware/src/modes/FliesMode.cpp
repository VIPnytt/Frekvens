#if MODE_FLIES

#include "modes/FliesMode.h"

#include "services/DisplayService.h"

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

void FliesMode::onReceive(JsonObjectConst payload,
                          std::string_view source) // NOLINT(misc-unused-parameters)
{
    if (payload["id"].is<uint8_t>() && payload["x"].is<uint8_t>() && payload["y"].is<uint8_t>())
    {
        flies[payload["id"].as<uint8_t>()] =
            std::pair<uint8_t, uint8_t>{payload["x"].as<uint8_t>(), payload["y"].as<uint8_t>()};
        pending = true;
    }
}

#endif // MODE_FLIES
