#if MODE_FLIES

#include "modes/FliesMode.h"

#include "services/DisplayService.h"

void FliesMode::handle()
{
    if (pending)
    {
        Display.clearFrame();
        for (const auto &[id, pixel] : flies)
        {
            Display.setPixel(pixel.x, pixel.y);
        }
        pending = false;
    }
}

void FliesMode::onReceive(JsonObjectConst payload, const char *source)
{
    if (payload["id"].is<uint8_t>() && payload["x"].is<uint8_t>() && payload["y"].is<uint8_t>())
    {
        flies[payload["id"].as<uint8_t>()] = Dot{payload["x"].as<uint8_t>(), payload["y"].as<uint8_t>()};
        pending = true;
    }
}

#endif // MODE_FLIES
