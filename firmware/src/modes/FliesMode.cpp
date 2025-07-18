#include "modes/FliesMode.h"
#include "services/DisplayService.h"

void FliesMode::handle()
{
    if (pending)
    {
        Display.clear();
        for (const auto &[id, pixel] : flies)
        {
            Display.setPixel(pixel.x, pixel.y);
        }
        pending = false;
    }
}

void FliesMode::receiverHook(const JsonDocument doc)
{
    if (doc["id"].is<uint8_t>() && doc["x"].is<uint8_t>() && doc["y"].is<uint8_t>())
    {
        flies[doc["id"].as<uint8_t>()] = Pixel{doc["x"].as<uint8_t>(), doc["y"].as<uint8_t>()};
        pending = true;
    }
}
