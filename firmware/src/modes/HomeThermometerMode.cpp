#if MODE_HOMETHERMOMETER

#include "modes/HomeThermometerMode.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "fonts/MiniFont.h"   // NOLINT(misc-include-cleaner)
#include "handlers/TextHandler.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"

#include <Preferences.h>
#include <nvs.h>
#include <regex>

void HomeThermometerMode::configure()
{
#if EXTENSION_HOMEASSISTANT
    const std::string topic{std::string("frekvens/" HOSTNAME "/").append(name)};
    const HomeAssistantExtension &_ha = Extensions.HomeAssistant();
    {
        for (const char *const where : {
                 "indoor",
                 "outdoor",
             })
        {
            const std::string id{std::regex_replace(name, std::regex(R"(\s+)"), "").append("_").append(where)};
            JsonObject component{(*_ha.discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
            component[HomeAssistantAbbreviations::command_template].set(
                std::string(R"({")").append(where).append(R"(":{{value}}})"));
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
            component[HomeAssistantAbbreviations::name].set((char)std::toupper(*where) + std::string(where + 1));
            component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
            component[HomeAssistantAbbreviations::platform].set("number");
            component[HomeAssistantAbbreviations::state_topic].set(topic);
            component[HomeAssistantAbbreviations::unique_id].set(_ha.uniquePrefix + id);
#ifdef TEMPERATURE_UNIT
            component[HomeAssistantAbbreviations::unit_of_measurement].set(TEMPERATURE_UNIT);
#endif // TEMPERATURE_UNIT
            component[HomeAssistantAbbreviations::value_template].set(
                std::string("{{value_json.").append(where).append("}}"));
        }
    }
#endif // EXTENSION_HOMEASSISTANT
    transmit();
}

void HomeThermometerMode::begin() { pending = true; }

void HomeThermometerMode::handle()
{
    if (pending)
    {
        update();
    }
}

void HomeThermometerMode::update()
{
    pending = false;

    Preferences Storage;
    Storage.begin(std::string(name).substr(0, NVS_KEY_NAME_MAX_SIZE - 1).c_str(), true);
    if (!Storage.isKey("indoor") || !Storage.isKey("outdoor"))
    {
        Storage.end();
        return;
    }
    const int16_t indoor = Storage.getShort("indoor");
    const int16_t outdoor = Storage.getShort("outdoor");
    Storage.end();
    const MiniFont font;
    const TextHandler _indoor{std::to_string(indoor).append("°"), font};
    const TextHandler _outdoor{std::to_string(outdoor).append("°"), font};
    const uint8_t _height = _outdoor.getHeight();
    const uint8_t marginsY = (GRID_ROWS - _indoor.getHeight() - _height) / 3;
    Display.clearFrame();
    _indoor.draw((GRID_COLUMNS - _indoor.getWidth()) / 2, marginsY);
    _outdoor.draw((GRID_COLUMNS - _outdoor.getWidth()) / 2, GRID_ROWS - marginsY - _height);
}

void HomeThermometerMode::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    Preferences Storage;
    Storage.begin(std::string(name).substr(0, NVS_KEY_NAME_MAX_SIZE - 1).c_str(), true);
    if (Storage.isKey("indoor"))
    {
        doc["indoor"].set(Storage.getShort("indoor"));
    }
    if (Storage.isKey("outdoor"))
    {
        doc["outdoor"].set(Storage.getShort("outdoor"));
    }
    Storage.end();
    if (doc.size() != 0)
    {
        Device.transmit(doc.as<JsonObjectConst>(), name);
    }
}

void HomeThermometerMode::onReceive(JsonObjectConst payload,
                                    std::string_view source) // NOLINT(misc-unused-parameters)
{
    if (payload["indoor"].is<float>())
    {
        setTemperature("indoor", round(payload["indoor"].as<float>()));
    }
    if (payload["outdoor"].is<float>())
    {
        setTemperature("outdoor", round(payload["outdoor"].as<float>()));
    }
}

void HomeThermometerMode::setTemperature(const char *where, int16_t temperature)
{
    Preferences Storage;
    Storage.begin(std::string(name).substr(0, NVS_KEY_NAME_MAX_SIZE - 1).c_str());
    Storage.putShort(where, temperature);
    Storage.end();
    pending = true;
    transmit();
#ifdef TEMPERATURE_UNIT
    ESP_LOGD(name, "%s %d" TEMPERATURE_UNIT, where, temperature); // NOLINT(cppcoreguidelines-avoid-do-while)
#else
    ESP_LOGD(name, "%s %d°", where, temperature); // NOLINT(cppcoreguidelines-avoid-do-while)
#endif // TEMPERATURE_UNIT
}

#endif // MODE_HOMETHERMOMETER
