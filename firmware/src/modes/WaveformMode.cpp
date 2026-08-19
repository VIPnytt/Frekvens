#if MODE_WAVEFORM

#include "modes/WaveformMode.h"

#include "handlers/BitmapHandler.h" // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"

#include <nvs.h>

static_assert(GRID_COLUMNS == 16U, __STRING(MODE_WAVEFORM) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 12U, __STRING(MODE_WAVEFORM) " is not compatible with this device's display size.");

/**
 * @brief Loads the saved wave selection and transmits the current configuration.
 */
void WaveformMode::configure()
{
    nvs_handle_t handle{};
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        uint8_t _wave{static_cast<uint8_t>(wave)};
        nvs_get_u8(handle, "wave", &_wave);
        nvs_close(handle);
        wave = static_cast<Wave>(_wave);
    }
    transmit();
}

/**
 * @brief Updates the display with a waveform or centered row at regular intervals.
 *
 * When microphone support is enabled, draws a waveform for triggered input;
 * otherwise, fills the center row. Without microphone support, always draws a
 * waveform.
 */
void WaveformMode::handle()
{
    if (millis() - lastMillis > (0b1U << 9U))
    {
        Display.fillFrame(0U);
#if EXTENSION_MICROPHONE
        if (Extensions.Microphone().isTriggered())
        {
            draw();
        }
        else
        {
            Display.fillRow(GRID_ROWS / 2U, UINT8_MAX);
        }
#else
        draw();
#endif // EXTENSION_MICROPHONE
        lastMillis = millis();
    }
}

void WaveformMode::draw()
{
    const std::span<const std::span<const uint16_t>> &_wave{waves[static_cast<size_t>(wave)]};
    BitmapHandler(_wave[random(_wave.size())]).draw();
}

void WaveformMode::setWave(std::string_view waveName)
{
    for (size_t idx{0U}; idx < waveNames.size(); ++idx)
    {
        if (waveName == waveNames[idx])
        {
            wave = static_cast<Wave>(idx);
            break;
        }
    }
    nvs_handle_t handle{};
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_u8(handle, "wave", static_cast<uint8_t>(wave));
        nvs_commit(handle);
        nvs_close(handle);
    }
    transmit();
}

/**
 * @brief Transmits the selected wave and available wave names.
 */
void WaveformMode::transmit()
{
    JsonDocument doc{};
    doc["wave"].set(waveNames[static_cast<uint8_t>(wave)]);
    JsonArray _waves{doc["waves"].to<JsonArray>()};
    for (const std::string_view _wave : waveNames)
    {
        _waves.add(_wave);
    }
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

/**
 * @brief Processes an incoming wave selection payload.
 *
 * @param payload Payload containing the wave selection.
 * @param source Origin of the payload.
 */
void WaveformMode::onReceive(JsonObjectConst payload, std::string_view source)
{
    // Wave
    if (payload["wave"].is<std::string_view>())
    {
        setWave(payload["wave"].as<std::string_view>());
    }
}

#if EXTENSION_HOMEASSISTANT
/**
 * @brief Adds a Home Assistant select entity for choosing the waveform.
 *
 * @param discovery Home Assistant discovery document to update.
 * @param topic Base topic used for the entity's command and state topics.
 * @param unique Prefix used to construct the entity's unique identifier.
 */
void WaveformMode::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique)
{
    topic.append(name);
    {
        const std::string id{std::string(name).append("_wave")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"wave":"{{value}}"})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::enabled_by_default].set(false);
        component[HomeAssistantAbbreviations::entity_category].set("config");
        component[HomeAssistantAbbreviations::icon].set("mdi:waveform");
        component[HomeAssistantAbbreviations::name].set(std::string(name).append(" wave"));
        JsonArray options{component[HomeAssistantAbbreviations::options].to<JsonArray>()};
        for (const std::string_view _wave : waveNames)
        {
            options.add(_wave);
        }
        component[HomeAssistantAbbreviations::platform].set("select");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.wave}}");
    }
}
#endif // EXTENSION_HOMEASSISTANT

#endif // MODE_WAVEFORM
