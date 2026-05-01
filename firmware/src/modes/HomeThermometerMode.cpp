#if MODE_HOMETHERMOMETER

#include "modes/HomeThermometerMode.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "fonts/MiniFont.h"   // NOLINT(misc-include-cleaner)
#include "handlers/TextHandler.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)

#include <nvs.h>

void HomeThermometerMode::configure()
{
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        nvs_get_i16(handle, "indoor", &indoor);
        nvs_get_i16(handle, "outdoor", &outdoor);
        nvs_close(handle);
    }
    transmit();
}

void HomeThermometerMode::begin() { pending = true; }

void HomeThermometerMode::handle()
{
    if (pending && (indoor != 0 || outdoor != 0))
    {
        draw();
    }
}

void HomeThermometerMode::draw()
{
    pending = false;
    const MiniFont font;
    const TextHandler textIndoor{std::to_string(indoor).append("°"), font};
    const TextHandler textOutdoor{std::to_string(outdoor).append("°"), font};
    const uint8_t height{textOutdoor.getHeight()};
    const uint8_t marginsY = (GRID_ROWS - (height * 2)) / 3;
    Display.clearFrame();
    textIndoor.draw((GRID_COLUMNS - textIndoor.getWidth()) / 2, marginsY);
    textOutdoor.draw((GRID_COLUMNS - textOutdoor.getWidth()) / 2, GRID_ROWS - marginsY - height);
}

void HomeThermometerMode::transmit()
{
    if (indoor != 0 || outdoor != 0)
    {
        JsonDocument doc; // NOLINT(misc-const-correctness)
        doc["indoor"].set(indoor);
        doc["outdoor"].set(outdoor);
        Device.transmit(doc.as<JsonObjectConst>(), name);
    }
}

void HomeThermometerMode::onReceive(JsonObjectConst payload,
                                    std::string_view source) // NOLINT(misc-unused-parameters)
{
    if (payload["indoor"].is<int16_t>()) // NOLINT(bugprone-branch-clone)
    {
        setTemperature("indoor", payload["indoor"].as<int16_t>());
    }
    else if (payload["indoor"].is<float>())
    {
        setTemperature("indoor", roundf(payload["indoor"].as<float>()));
    }
    if (payload["outdoor"].is<int16_t>()) // NOLINT(bugprone-branch-clone)
    {
        setTemperature("outdoor", payload["outdoor"].as<int16_t>());
    }
    else if (payload["outdoor"].is<float>())
    {
        setTemperature("outdoor", roundf(payload["outdoor"].as<float>()));
    }
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void HomeThermometerMode::setTemperature(std::string_view where, int16_t temperature)
{
    if (where == "indoor")
    {
        indoor = temperature;
    }
    else if (where == "outdoor")
    {
        outdoor = temperature;
    }
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_i16(handle, "indoor", indoor);
        nvs_set_i16(handle, "outdoor", outdoor);
        nvs_commit(handle);
        nvs_close(handle);
    }
    pending = true;
    transmit();
}

#if EXTENSION_HOMEASSISTANT
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void HomeThermometerMode::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique)
{
    topic.append(name);
    {
        for (const auto &where : {
                 std::pair<const char *, const char *>{"indoor", "Indoor"},
                 std::pair<const char *, const char *>{"outdoor", "Outdoor"},
             })
        {
            const std::string id{std::string("Homethermometer_").append(where.first)};
            JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
            component[HomeAssistantAbbreviations::command_template].set(
                std::string(R"({")").append(where.first).append(R"(":{{value}}})"));
            component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
            component[HomeAssistantAbbreviations::device_class].set("temperature");
            component[HomeAssistantAbbreviations::entity_category].set("config");
            component[HomeAssistantAbbreviations::icon].set("mdi:thermometer");
#if GRID_COLUMNS < 18
            component[HomeAssistantAbbreviations::max].set(999);
            component[HomeAssistantAbbreviations::min].set(-99);
#elif GRID_COLUMNS < 22
            component[HomeAssistantAbbreviations::max].set(9999);
            component[HomeAssistantAbbreviations::min].set(-999);
#elif GRID_COLUMNS < 26
            component[HomeAssistantAbbreviations::max].set(INT16_MAX);
            component[HomeAssistantAbbreviations::min].set(-9999);
#else
            component[HomeAssistantAbbreviations::max].set(INT16_MAX);
            component[HomeAssistantAbbreviations::min].set(INT16_MIN);
#endif // GRID_COLUMNS < 18
            component[HomeAssistantAbbreviations::mode].set("box");
            component[HomeAssistantAbbreviations::name].set(where.second);
            component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
            component[HomeAssistantAbbreviations::platform].set("number");
            component[HomeAssistantAbbreviations::state_topic].set(topic);
            component[HomeAssistantAbbreviations::unique_id].set(unique + id);
#ifdef TEMPERATURE_UNIT
            component[HomeAssistantAbbreviations::unit_of_measurement].set(TEMPERATURE_UNIT);
#endif // TEMPERATURE_UNIT
            component[HomeAssistantAbbreviations::value_template].set(
                std::string("{{value_json.").append(where.first).append("}}"));
        }
    }
}
#endif // EXTENSION_HOMEASSISTANT

#endif // MODE_HOMETHERMOMETER
