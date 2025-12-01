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

void FliesMode::onReceive(const JsonDocument doc, const char *const source)
{
    if (doc["id"].is<uint8_t>() && doc["x"].is<uint8_t>() && doc["y"].is<uint8_t>())
    {
        flies[doc["id"].as<uint8_t>()] = Dot{doc["x"].as<uint8_t>(), doc["y"].as<uint8_t>()};
        pending = true;
    }
}

#endif // MODE_FLIES
