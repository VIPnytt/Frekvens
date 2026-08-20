#if MODE_HOMETHERMOMETER

#include "modes/HomeThermometerMode.h"

#include "config/constants.h"                  // NOLINT(misc-include-cleaner)
#include "extensions/HomeAssistantExtension.h" // NOLINT(misc-include-cleaner)
#include "fonts/MiniFont.h"                    // NOLINT(misc-include-cleaner)
#include "handlers/TextHandler.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"

#include <nvs.h>

static_assert(GRID_COLUMNS >= 6U, __STRING(MODE_HOMETHERMOMETER) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 11U, __STRING(MODE_HOMETHERMOMETER) " is not compatible with this device's display size.");

/**
 * @brief Loads saved indoor and outdoor temperatures, then publishes the current readings.
 */
void HomeThermometerMode::configure()
{
    nvs_handle_t handle{};
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        nvs_get_i16(handle, "indoor", &indoor);
        nvs_get_i16(handle, "outdoor", &outdoor);
        nvs_close(handle);
    }
    transmit();
}

void HomeThermometerMode::begin() { pending = true; }

/**
 * @brief Updates the display when a temperature change is pending.
 */
void HomeThermometerMode::handle()
{
    if (pending && (indoor != 0 || outdoor != 0))
    {
        draw();
    }
}

/**
 * @brief Renders the indoor and outdoor temperatures centered on the display.
 */
void HomeThermometerMode::draw()
{
    pending = false;
    const MiniFont font;
    const TextHandler textIndoor{std::to_string(indoor).append("°"), font};
    const TextHandler textOutdoor{std::to_string(outdoor).append("°"), font};
    const uint8_t height{textOutdoor.getHeight()};
    const uint8_t marginsY{static_cast<uint8_t>((GRID_ROWS - (height * 2U)) / 3U)};
    Display.fillFrame(0U);
    textIndoor.draw(static_cast<int16_t>((GRID_COLUMNS - textIndoor.getWidth()) / 2), static_cast<int8_t>(marginsY));
    textOutdoor.draw(static_cast<int16_t>((GRID_COLUMNS - textOutdoor.getWidth()) / 2), GRID_ROWS - marginsY - height);
}

/**
 * @brief Transmits the current indoor and outdoor temperatures when either value is set.
 */
void HomeThermometerMode::transmit()
{
    if (indoor != 0 || outdoor != 0)
    {
        JsonDocument doc{};
        doc["indoor"].set(indoor);
        doc["outdoor"].set(outdoor);
        Device.transmit(doc.as<JsonObjectConst>(), name);
    }
}

/**
 * @brief Updates indoor and outdoor temperatures from a JSON payload.
 *
 * @param payload JSON object containing optional indoor and outdoor temperature values.
 *        Floating-point values are rounded before being stored.
 */
void HomeThermometerMode::onReceive(JsonObjectConst payload, std::string_view source)
{
    if (payload["indoor"].is<int16_t>())
    {
        setTemperature("indoor", payload["indoor"].as<int16_t>());
    }
    else if (payload["indoor"].is<float>())
    {
        setTemperature("indoor", lroundf(payload["indoor"].as<float>()));
    }
    if (payload["outdoor"].is<int16_t>())
    {
        setTemperature("outdoor", payload["outdoor"].as<int16_t>());
    }
    else if (payload["outdoor"].is<float>())
    {
        setTemperature("outdoor", lroundf(payload["outdoor"].as<float>()));
    }
}

/**
 * @brief Updates an indoor or outdoor temperature and publishes the current values.
 *
 * @param where Selects the temperature to update; supported values are `"indoor"` and `"outdoor"`.
 * @param temperature New temperature value.
 */
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
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
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
/**
 * @brief Configures Home Assistant entities for indoor and outdoor temperatures.
 *
 * @param discovery Home Assistant discovery document to populate.
 * @param topic Base topic for temperature state and command messages.
 * @param unique Prefix used to generate unique entity identifiers.
 */
void HomeThermometerMode::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique)
{
    topic.append(name);
    {
        for (const std::pair<const char *, const char *> &where : {
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
#if GRID_COLUMNS >= 26
            component[HomeAssistantAbbreviations::max].set(INT16_MAX);
            component[HomeAssistantAbbreviations::min].set(INT16_MIN);
#elif GRID_COLUMNS >= 22
            component[HomeAssistantAbbreviations::max].set(INT16_MAX);
            component[HomeAssistantAbbreviations::min].set(-9999);
#elif GRID_COLUMNS >= 18
            component[HomeAssistantAbbreviations::max].set(9999U);
            component[HomeAssistantAbbreviations::min].set(-999);
#elif GRID_COLUMNS >= 14
            component[HomeAssistantAbbreviations::max].set(999U);
            component[HomeAssistantAbbreviations::min].set(-99);
#elif GRID_COLUMNS >= 10
            component[HomeAssistantAbbreviations::max].set(99U);
            component[HomeAssistantAbbreviations::min].set(-9);
#else
            component[HomeAssistantAbbreviations::max].set(9U);
            component[HomeAssistantAbbreviations::min].set(0);
#endif // GRID_COLUMNS >= 26
            component[HomeAssistantAbbreviations::mode].set("box");
            component[HomeAssistantAbbreviations::name].set(where.second);
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
