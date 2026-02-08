#include "config/constants.h"

#if MODE_HOMETHERMOMETER

#include "extensions/HomeAssistantExtension.h"
#include "fonts/MiniFont.h"
#include "handlers/TextHandler.h"
#include "modes/HomeThermometerMode.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

#include <Preferences.h>
#include <nvs.h>
#include <regex>

void HomeThermometerMode::configure()
{
#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        for (const char *const where : {
                 "indoor",
                 "outdoor",
             })
        {
            const std::string id = std::regex_replace(name, std::regex("\\s+"), "").append("_").append(where);
            JsonObject component =
                (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
            component[HomeAssistantAbbreviations::command_template] =
                std::string("{\"").append(where).append("\":{{value}}}");
            component[HomeAssistantAbbreviations::command_topic] = topic + "/set";
            component[HomeAssistantAbbreviations::device_class] = "temperature";
            component[HomeAssistantAbbreviations::entity_category] = "config";
            component[HomeAssistantAbbreviations::icon] = "mdi:thermometer";
#if GRID_COLUMNS < 18
            component[HomeAssistantAbbreviations::max] = 999;
            component[HomeAssistantAbbreviations::min] = -99;
#elif GRID_COLUMNS < 22
            component[HomeAssistantAbbreviations::max] = 9999;
            component[HomeAssistantAbbreviations::min] = -999;
#elif GRID_COLUMNS < 26
            component[HomeAssistantAbbreviations::max] = INT16_MAX;
            component[HomeAssistantAbbreviations::min] = -9999;
#else
            component[HomeAssistantAbbreviations::max] = INT16_MAX;
            component[HomeAssistantAbbreviations::min] = INT16_MIN;
#endif // GRID_COLUMNS < 18
            component[HomeAssistantAbbreviations::mode] = "box";
            component[HomeAssistantAbbreviations::name] = (char)std::toupper(*where) + std::string(where + 1);
            component[HomeAssistantAbbreviations::object_id] = HOSTNAME "_" + id;
            component[HomeAssistantAbbreviations::platform] = "number";
            component[HomeAssistantAbbreviations::state_topic] = topic;
            component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
#if TEMPERATURE_KELVIN
            component[HomeAssistantAbbreviations::unit_of_measurement] = "°K";
#elif TEMPERATURE_CELSIUS
            component[HomeAssistantAbbreviations::unit_of_measurement] = "°C";
#elif TEMPERATURE_FAHRENHEIT
            component[HomeAssistantAbbreviations::unit_of_measurement] = "°F";
#endif // TEMPERATURE_KELVIN
            component[HomeAssistantAbbreviations::value_template] =
                std::string("{{value_json.").append(where).append("}}");
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
    const int16_t indoor = Storage.getShort("indoor"), outdoor = Storage.getShort("outdoor");
    Storage.end();
    TextHandler _indoor = TextHandler(std::to_string(indoor).append("°"), FontMini),
                _outdoor = TextHandler(std::to_string(outdoor).append("°"), FontMini);
    const uint8_t _height = _outdoor.getHeight(), marginsY = (GRID_ROWS - _indoor.getHeight() - _height) / 3;
    Display.clearFrame();
    _indoor.draw((GRID_COLUMNS - _indoor.getWidth()) / 2, marginsY);
    _outdoor.draw((GRID_COLUMNS - _outdoor.getWidth()) / 2, GRID_ROWS - marginsY - _height);
}

void HomeThermometerMode::transmit()
{
    JsonDocument doc;
    Preferences Storage;
    Storage.begin(std::string(name).substr(0, NVS_KEY_NAME_MAX_SIZE - 1).c_str(), true);
    if (Storage.isKey("indoor"))
    {
        doc["indoor"] = Storage.getShort("indoor");
    }
    if (Storage.isKey("outdoor"))
    {
        doc["outdoor"] = Storage.getShort("outdoor");
    }
    Storage.end();
    if (doc.size())
    {
        Device.transmit(doc, name);
    }
}

void HomeThermometerMode::onReceive(const JsonDocument doc, const char *const source)
{
    if (doc["indoor"].is<float>())
    {
        setTemperature("indoor", round(doc["indoor"].as<float>()));
    }
    if (doc["outdoor"].is<float>())
    {
        setTemperature("outdoor", round(doc["outdoor"].as<float>()));
    }
}

void HomeThermometerMode::setTemperature(const char *const where, const int16_t temperature)
{
    Preferences Storage;
    Storage.begin(std::string(name).substr(0, NVS_KEY_NAME_MAX_SIZE - 1).c_str());
    Storage.putShort(where, temperature);
    Storage.end();
    pending = true;
    transmit();
#if TEMPERATURE_KELVIN
    ESP_LOGD(name, "%s %d°K", where, temperature);
#elif TEMPERATURE_CELSIUS
    ESP_LOGD(name, "%s %d°C", where, temperature);
#elif TEMPERATURE_FAHRENHEIT
    ESP_LOGD(name, "%s %d°F", where, temperature);
#else
    ESP_LOGD(name, "%s %d°", where, temperature);
#endif // TEMPERATURE_KELVIN
}

#endif // MODE_HOMETHERMOMETER
