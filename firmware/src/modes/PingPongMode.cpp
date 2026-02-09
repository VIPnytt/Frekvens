#if MODE_PINGPONG

#include "modes/PingPongMode.h"

#include "extensions/HomeAssistantExtension.h"
#include "fonts/MiniFont.h"
#include "handlers/TextHandler.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

#include <Preferences.h>

void PingPongMode::configure()
{
#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_clock");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::command_template] = "{\"clock\":{{value}}}";
        component[HomeAssistantAbbreviations::command_topic] = topic + "/set";
        component[HomeAssistantAbbreviations::enabled_by_default] = false;
        component[HomeAssistantAbbreviations::entity_category] = "config";
        component[HomeAssistantAbbreviations::icon] = "mdi:table-tennis";
        component[HomeAssistantAbbreviations::name] = std::string(name).append(" clock");
        component[HomeAssistantAbbreviations::object_id] = HOSTNAME "_" + id;
        component[HomeAssistantAbbreviations::payload_off] = "false";
        component[HomeAssistantAbbreviations::payload_on] = "true";
        component[HomeAssistantAbbreviations::platform] = "switch";
        component[HomeAssistantAbbreviations::state_off] = "False";
        component[HomeAssistantAbbreviations::state_on] = "True";
        component[HomeAssistantAbbreviations::state_topic] = topic;
        component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[HomeAssistantAbbreviations::value_template] = "{{value_json.clock}}";
    }
#endif // EXTENSION_HOMEASSISTANT
    Preferences Storage;
    Storage.begin(name, true);
    if (Storage.isKey("clock"))
    {
        clock = Storage.getBool("clock");
    }
    Storage.end();
    transmit();
}

void PingPongMode::begin()
{
    pending = true;
    Display.clearFrame();
    paddleA.clear();
    paddleB.clear();
#if GRID_COLUMNS == GRID_ROWS
    const uint8_t _paddle = random(clock ? 5 : 0, GRID_COLUMNS - 3);
#else
    const uint8_t _paddle = random(clock ? 5 : 0, clock ? GRID_ROWS - 3 : GRID_COLUMNS - 3);
#endif // GRID_COLUMNS == GRID_ROWS
    for (uint8_t i = 0; i < 3; ++i)
    {
        paddleA.push_back(_paddle + i);
        paddleB.push_back(_paddle + i);
        Display.setPixel(clock ? 0 : _paddle + i, clock ? _paddle + i : 0);
        Display.setPixel(clock ? GRID_ROWS - 1 : _paddle + i, clock ? _paddle + i : GRID_ROWS - 1);
    }
    xDec = x = clock ? GRID_COLUMNS - 2 : _paddle + 1;
    yDec = y = clock ? _paddle + 1 : GRID_ROWS - 2;
    deg = random(clock ? 150 : 60, clock ? 211 : 121); // ±30°
    Display.setPixel(x, y);
}

void PingPongMode::handle()
{
    if (clock && getLocalTime(&local) && (minute != local.tm_min || hour != local.tm_hour || pending))
    {
        hour = local.tm_hour;
        minute = local.tm_min;
        Display.drawRectangle(0, 0, GRID_COLUMNS - 1, 4, true, 0);
        TextHandler(std::to_string(hour / 10), FontMini).draw(GRID_COLUMNS / 2 - 8, 0);
        TextHandler(std::to_string(hour % 10), FontMini).draw(GRID_COLUMNS / 2 - 4, 0);
        TextHandler(std::to_string(minute / 10), FontMini).draw(GRID_COLUMNS / 2 + 1, 0);
        TextHandler(std::to_string(minute % 10), FontMini).draw(GRID_COLUMNS / 2 + 5, 0);
        pending = false;
    }
    if (!clock && yDec >= GRID_ROWS - 2 && deg >= 180)
    {
        // Bottom
        deg = random(45, 136); // ±45°
    }
    else if (clock && xDec <= 1 && deg >= 90 && deg < 270)
    {
        // Left
        deg = (random(45, 136) + 270) % 360; // ±45°
    }
    else if (clock && xDec >= GRID_COLUMNS - 2 && (deg < 90 || deg >= 270))
    {
        // Right
        deg = random(135, 226); // ±45°
    }
    else if (!clock && yDec <= 1 && deg < 180)
    {
        // Top
        deg = random(225, 316); // ±45°
    }
    if (!clock && xDec <= 0 || xDec >= GRID_COLUMNS - 1)
    {
        // Left/right
        deg = deg >= 180 ? 540 - deg : 180 - deg; // Invert X
    }
    else if (clock && yDec <= 5 || yDec >= GRID_ROWS - 1)
    {
        // Top/bottom
        deg = 360 - deg; // Invert Y
    }
    Display.setPixel(x, y, 0);
    xDec += cosf(deg * DEG_TO_RAD) * speed;
    yDec -= sinf(deg * DEG_TO_RAD) * speed;
    x = lroundf(xDec);
    y = lroundf(yDec);
    Display.setPixel(x, y, clock ? INT8_MAX : UINT8_MAX);
    const float aRad = atanf(clock ? (xDec - 1) / abs(paddleA[1] - yDec) : (yDec - 1) / abs(paddleB[1] - xDec));
    const float bRad = atanf(clock ? (GRID_COLUMNS - 2 - xDec) / abs(paddleB[1] - yDec)
                                   : (GRID_ROWS - 2 - yDec) / abs(paddleA[1] - xDec));
    if (!clock && xDec < paddleA.front() && bRad < 1 && paddleA.front() > 0)
    {
        // Bottom left
        Display.setPixel(paddleA.back(), GRID_ROWS - 1, 0);
        paddleA.pop_back();
        paddleA.push_front(paddleA.front() - 1);
        Display.setPixel(paddleA.front(), GRID_ROWS - 1);
    }
    else if (!clock && xDec > paddleA.back() && bRad < 1 && paddleA.back() < GRID_COLUMNS - 1)
    {
        // Bottom right
        Display.setPixel(paddleA.front(), GRID_ROWS - 1, 0);
        paddleA.pop_front();
        paddleA.push_back(paddleA.back() + 1);
        Display.setPixel(paddleA.back(), GRID_ROWS - 1);
    }
    else if (clock && yDec > paddleA.back() && aRad < 1 && paddleA.back() < GRID_ROWS - 1)
    {
        // Left down
        Display.setPixel(0, paddleA.front(), 0);
        paddleA.pop_front();
        paddleA.push_back(paddleA.back() + 1);
        Display.setPixel(0, paddleA.back(), INT8_MAX);
    }
    else if (clock && yDec < paddleA.front() && aRad < 1 && paddleA.front() > 5)
    {
        // Left up
        Display.setPixel(0, paddleA.back(), 0);
        paddleA.pop_back();
        paddleA.push_front(paddleA.front() - 1);
        Display.setPixel(0, paddleA.front(), INT8_MAX);
    }
    else if (clock && yDec > paddleB.back() && bRad < 1 && paddleB.back() < GRID_ROWS - 1)
    {
        // Right down
        Display.setPixel(GRID_COLUMNS - 1, paddleB.front(), 0);
        paddleB.pop_front();
        paddleB.push_back(paddleB.back() + 1);
        Display.setPixel(GRID_COLUMNS - 1, paddleB.back(), INT8_MAX);
    }
    else if (clock && yDec < paddleB.front() && bRad < 1 && paddleB.front() > 5)
    {
        // Right up
        Display.setPixel(GRID_COLUMNS - 1, paddleB.back(), 0);
        paddleB.pop_back();
        paddleB.push_front(paddleB.front() - 1);
        Display.setPixel(GRID_COLUMNS - 1, paddleB.front(), INT8_MAX);
    }
    else if (!clock && xDec < paddleB.front() && aRad < 1 && paddleB.front() > 0)
    {
        // Top left
        Display.setPixel(paddleB.back(), 0, 0);
        paddleB.pop_back();
        paddleB.push_front(paddleB.front() - 1);
        Display.setPixel(paddleB.front(), 0);
    }
    else if (!clock && xDec > paddleB.back() && aRad < 1 && paddleB.back() < GRID_COLUMNS - 1)
    {
        // Top right
        Display.setPixel(paddleB.front(), 0, 0);
        paddleB.pop_front();
        paddleB.push_back(paddleB.back() + 1);
        Display.setPixel(paddleB.back(), 0);
    }
}

void PingPongMode::setClock(bool _clock)
{
    if (_clock != clock)
    {
        clock = _clock;
        if (clock && yDec <= 5)
        {
            yDec = 5.5f;
        }
        Display.clearFrame();
        Preferences Storage;
        Storage.begin(name);
        Storage.putBool("clock", clock);
        Storage.end();
        pending = true;
        transmit();
    }
}

void PingPongMode::transmit()
{
    JsonDocument doc;
    doc["clock"] = clock;
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void PingPongMode::onReceive(JsonObjectConst payload, const char *source)
{
    // Clock
    if (payload["clock"].is<bool>())
    {
        setClock(payload["clock"].as<bool>());
    }
}

#endif // MODE_PINGPONG
