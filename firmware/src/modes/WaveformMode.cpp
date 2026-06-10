#if MODE_WAVEFORM

#include "modes/WaveformMode.h"

#include "handlers/BitmapHandler.h" // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"

#include <nvs.h>

static_assert(GRID_COLUMNS == 16U, __STRING(MODE_WAVEFORM) " is not compatible with this device's display size.");
static_assert(GRID_ROWS >= 12U, __STRING(MODE_WAVEFORM) " is not compatible with this device's display size.");

void WaveformMode::configure()
{
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        uint8_t _wave{static_cast<uint8_t>(wave)};
        nvs_get_u8(handle, "wave", &_wave);
        nvs_close(handle);
        wave = static_cast<Wave>(_wave);
    }
    transmit();
}

void WaveformMode::handle()
{
    if (millis() - lastMillis > (0b1U << 9U))
    {
        Display.clearFrame();
#if EXTENSION_MICROPHONE
        if (Extensions.Microphone().isTriggered())
        {
            draw();
        }
        else
        {
            for (uint8_t x{0U}; x < GRID_COLUMNS; ++x)
            {
                Display.setPixel(x, GRID_ROWS / 2U);
            }
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
    for (size_t i{0U}; i < waveNames.size(); ++i)
    {
        if (waveName == waveNames[i])
        {
            wave = static_cast<Wave>(i);
            break;
        }
    }
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_u8(handle, "wave", static_cast<uint8_t>(wave));
        nvs_commit(handle);
        nvs_close(handle);
    }
    transmit();
}

void WaveformMode::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["wave"].set(waveNames[static_cast<uint8_t>(wave)]);
    JsonArray _waves{doc["waves"].to<JsonArray>()};
    for (const std::string_view _wave : waveNames)
    {
        _waves.add(_wave);
    }
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void WaveformMode::onReceive(JsonObjectConst payload,
                             std::string_view source) // NOLINT(misc-unused-parameters)
{
    // Wave
    if (payload["wave"].is<std::string_view>())
    {
        setWave(payload["wave"].as<std::string_view>());
    }
}

#if EXTENSION_HOMEASSISTANT
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
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
