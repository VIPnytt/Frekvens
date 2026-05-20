#if MODE_PINGPONG

#include "modes/PingPongMode.h"

#include "fonts/MiniFont.h"       // NOLINT(misc-include-cleaner)
#include "handlers/TextHandler.h" // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)

#include <nvs.h>

static_assert(GRID_COLUMNS >= 16U, __STRING(MODE_PINGPONG) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 9U, __STRING(MODE_PINGPONG) " is not compatible with this device's display size.");

void PingPongMode::configure()
{
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        uint8_t _clock{0};
        if (nvs_get_u8(handle, "clock", &_clock) == ESP_OK)
        {
            clock = static_cast<bool>(_clock);
        }
        nvs_close(handle);
    }
    transmit();
}

void PingPongMode::begin()
{
    pending = true;
    Display.clearFrame();
    paddleA.clear();
    paddleB.clear();
#if GRID_COLUMNS == GRID_ROWS
    const uint8_t _paddle{static_cast<uint8_t>(random(clock ? 5 : 0, GRID_COLUMNS - 3U))};
#else
    const uint8_t _paddle{random(clock ? 5 : 0, clock ? GRID_ROWS - 3U : GRID_COLUMNS - 3U)};
#endif // GRID_COLUMNS == GRID_ROWS
    for (uint8_t i{0U}; i < 3U; ++i)
    {
        paddleA.push_back(_paddle + i);
        paddleB.push_back(_paddle + i);
        Display.setPixel(clock ? 0 : _paddle + i, clock ? _paddle + i : 0U);
        Display.setPixel(clock ? GRID_ROWS - 1U : _paddle + i, clock ? _paddle + i : GRID_ROWS - 1U);
    }
    xDec = x = clock ? GRID_COLUMNS - 2U : _paddle + 1U;
    yDec = y = clock ? _paddle + 1U : GRID_ROWS - 2U;
    deg = random(clock ? 150 : 60, clock ? 211 : 121); // ±30°
    Display.setPixel(x, y);
}

void PingPongMode::handle()
{
    if (clock && getLocalTime(&local) && (minute != local.tm_min || hour != local.tm_hour || pending))
    {
        hour = local.tm_hour;
        minute = local.tm_min;
#ifdef CLOCK_12H
        const int hour{(local.tm_hour + 11) % 12 + 1};
#endif // CLOCK_12H
        Display.drawRectangle((GRID_COLUMNS / 2U) - 8U, 0U, (GRID_COLUMNS / 2U) + 7U, 4U, true, 0U);
        const MiniFont font;
        TextHandler(std::to_string(hour / 10), font).draw(GRID_COLUMNS / 2U - 8U, 0U);
        TextHandler(std::to_string(hour % 10), font).draw(GRID_COLUMNS / 2U - 4U, 0U);
        TextHandler(std::to_string(minute / 10), font).draw(GRID_COLUMNS / 2U + 1U, 0U);
        TextHandler(std::to_string(minute % 10), font).draw(GRID_COLUMNS / 2U + 5U, 0U);
        pending = false;
    }
    if (!clock && yDec >= GRID_ROWS - 2U && deg >= 180U)
    {
        // Bottom
        deg = random(45, 136); // ±45°
    }
    else if (clock && xDec <= 1 && deg >= 90U && deg < 270U)
    {
        // Left
        deg = (random(45, 136) + 270U) % 360U; // ±45°
    }
    else if (clock && xDec >= GRID_COLUMNS - 2U && (deg < 90U || deg >= 270U))
    {
        // Right
        deg = random(135, 226); // ±45°
    }
    else if (!clock && yDec <= 1U && deg < 180U)
    {
        // Top
        deg = random(225, 316); // ±45°
    }
    if (!clock && xDec <= 0U || xDec >= GRID_COLUMNS - 1U)
    {
        // Left/right
        deg = deg >= 180U ? 540U - deg : 180U - deg; // Invert X
    }
    else if (clock && yDec <= 5U || yDec >= GRID_ROWS - 1U)
    {
        // Top/bottom
        deg = 360U - deg; // Invert Y
    }
    Display.setPixel(x, y, 0U);
    xDec += cosf(static_cast<float>(deg) * static_cast<float>(DEG_TO_RAD)) * speed;
    yDec -= sinf(static_cast<float>(deg) * static_cast<float>(DEG_TO_RAD)) * speed;
    x = lroundf(xDec);
    y = lroundf(yDec);
    Display.setPixel(x, y, clock ? INT8_MAX : UINT8_MAX);
    const float aRad{atanf(clock ? (xDec - 1U) / abs(paddleA[1U] - yDec) : (yDec - 1U) / abs(paddleB[1U] - xDec))};
    const float bRad{atanf(clock ? (GRID_COLUMNS - 2U - xDec) / abs(paddleB[1U] - yDec)
                                 : (GRID_ROWS - 2U - yDec) / abs(paddleA[1U] - xDec))};
    if (!clock && xDec < paddleA.front() && bRad < 1U && paddleA.front() != 0U)
    {
        // Bottom left
        Display.setPixel(paddleA.back(), GRID_ROWS - 1U, 0U);
        paddleA.pop_back();
        paddleA.push_front(paddleA.front() - 1U);
        Display.setPixel(paddleA.front(), GRID_ROWS - 1U);
    }
    else if (!clock && xDec > paddleA.back() && bRad < 1U && paddleA.back() < GRID_COLUMNS - 1U)
    {
        // Bottom right
        Display.setPixel(paddleA.front(), GRID_ROWS - 1U, 0U);
        paddleA.pop_front();
        paddleA.push_back(paddleA.back() + 1U);
        Display.setPixel(paddleA.back(), GRID_ROWS - 1U);
    }
    else if (clock && yDec > paddleA.back() && aRad < 1U && paddleA.back() < GRID_ROWS - 1U)
    {
        // Left down
        Display.setPixel(0U, paddleA.front(), 0U);
        paddleA.pop_front();
        paddleA.push_back(paddleA.back() + 1U);
        Display.setPixel(0U, paddleA.back(), INT8_MAX);
    }
    else if (clock && yDec < paddleA.front() && aRad < 1U && paddleA.front() > 5U)
    {
        // Left up
        Display.setPixel(0U, paddleA.back(), 0U);
        paddleA.pop_back();
        paddleA.push_front(paddleA.front() - 1U);
        Display.setPixel(0U, paddleA.front(), INT8_MAX);
    }
    else if (clock && yDec > paddleB.back() && bRad < 1U && paddleB.back() < GRID_ROWS - 1U)
    {
        // Right down
        Display.setPixel(GRID_COLUMNS - 1U, paddleB.front(), 0);
        paddleB.pop_front();
        paddleB.push_back(paddleB.back() + 1U);
        Display.setPixel(GRID_COLUMNS - 1U, paddleB.back(), INT8_MAX);
    }
    else if (clock && yDec < paddleB.front() && bRad < 1U && paddleB.front() > 5U)
    {
        // Right up
        Display.setPixel(GRID_COLUMNS - 1U, paddleB.back(), 0U);
        paddleB.pop_back();
        paddleB.push_front(paddleB.front() - 1U);
        Display.setPixel(GRID_COLUMNS - 1U, paddleB.front(), INT8_MAX);
    }
    else if (!clock && xDec < paddleB.front() && aRad < 1U && paddleB.front() != 0U)
    {
        // Top left
        Display.setPixel(paddleB.back(), 0U, 0U);
        paddleB.pop_back();
        paddleB.push_front(paddleB.front() - 1U);
        Display.setPixel(paddleB.front(), 0U);
    }
    else if (!clock && xDec > paddleB.back() && aRad < 1U && paddleB.back() < GRID_COLUMNS - 1U)
    {
        // Top right
        Display.setPixel(paddleB.front(), 0U, 0U);
        paddleB.pop_front();
        paddleB.push_back(paddleB.back() + 1U);
        Display.setPixel(paddleB.back(), 0U);
    }
}

void PingPongMode::setClock(bool _clock)
{
    clock = _clock;
    if (clock && yDec <= 5.0F)
    {
        yDec = 5.5F;
    }
    Display.clearFrame();
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_u8(handle, "clock", static_cast<uint8_t>(clock)); // NOLINT(readability-implicit-bool-conversion)
        nvs_commit(handle);
        nvs_close(handle);
    }
    pending = true;
    transmit();
}

void PingPongMode::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["clock"].set(clock);
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void PingPongMode::onReceive(JsonObjectConst payload,
                             std::string_view source) // NOLINT(misc-unused-parameters)
{
    // Clock
    if (payload["clock"].is<bool>())
    {
        setClock(payload["clock"].as<bool>());
    }
}

#if EXTENSION_HOMEASSISTANT
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void PingPongMode::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique)
{
    topic.append(name);
    {
        const std::string id{std::string(name).append("_clock")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"clock":{{value}}})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::enabled_by_default].set(false);
        component[HomeAssistantAbbreviations::entity_category].set("config");
        component[HomeAssistantAbbreviations::icon].set("mdi:table-tennis");
        component[HomeAssistantAbbreviations::name].set(std::string(name).append(" clock"));
        component[HomeAssistantAbbreviations::payload_off].set("false");
        component[HomeAssistantAbbreviations::payload_on].set("true");
        component[HomeAssistantAbbreviations::platform].set("switch");
        component[HomeAssistantAbbreviations::state_off].set("False");
        component[HomeAssistantAbbreviations::state_on].set("True");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.clock}}");
    }
}
#endif // EXTENSION_HOMEASSISTANT

#endif // MODE_PINGPONG
