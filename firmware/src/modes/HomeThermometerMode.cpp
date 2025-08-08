#include <nvs.h>
#include <Preferences.h>
#include <regex>

#include "config/constants.h"
#include "extensions/BuildExtension.h"
#include "extensions/HomeAssistantExtension.h"
#include "extensions/MqttExtension.h"
#include "fonts/MiniFont.h"
#include "handlers/TextHandler.h"
#include "modes/HomeThermometerMode.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

void HomeThermometerMode::setup()
{
#if EXTENSION_BUILD && defined(HOMETHERMOMETER_UNIT)
    (*Build->config)[Config::h][__STRING(HOMETHERMOMETER_UNIT)] = HOMETHERMOMETER_UNIT;
#endif // EXTENSION_BUILD && defined(HOMETHERMOMETER_UNIT)
#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        for (const char *const where : {
                 "indoor",
                 "outdoor",
             })
        {
            const std::string id = std::regex_replace(name, std::regex("\\s+"), "").append("_").append(where);
            JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
            component[Abbreviations::command_template] = std::string("{\"").append(where).append("\":{{value}}}");
            component[Abbreviations::command_topic] = topic + "/set";
            component[Abbreviations::device_class] = "temperature";
            component[Abbreviations::entity_category] = "config";
            component[Abbreviations::icon] = "mdi:thermometer";
#if COLUMNS < 18
            component[Abbreviations::max] = 999;
            component[Abbreviations::min] = -99;
#elif COLUMNS < 22
            component[Abbreviations::max] = 9999;
            component[Abbreviations::min] = -999;
#elif COLUMNS < 26
            component[Abbreviations::max] = INT16_MAX;
            component[Abbreviations::min] = -9999;
#else
            component[Abbreviations::max] = INT16_MAX;
            component[Abbreviations::min] = INT16_MIN;
#endif // COLUMNS < 18
            component[Abbreviations::mode] = "box";
            component[Abbreviations::name] = (char)std::toupper(*where) + std::string(where + 1);
            component[Abbreviations::object_id] = HOSTNAME "_" + id;
            component[Abbreviations::platform] = "number";
            component[Abbreviations::state_topic] = topic;
            component[Abbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
#ifdef HOMETHERMOMETER_UNIT
            component[Abbreviations::unit_of_measurement] = HOMETHERMOMETER_UNIT;
#else
            component[Abbreviations::unit_of_measurement] = "°C";
#endif // HOMETHERMOMETER_UNIT
            component[Abbreviations::value_template] = std::string("{{value_json.").append(where).append("}}");
        }
    }
#endif // EXTENSION_HOMEASSISTANT
    transmit();
}

void HomeThermometerMode::wake()
{
    pending = true;
}

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
    int16_t
        indoorInt = Storage.getShort("indoor"),
        outdoorInt = Storage.getShort("outdoor");
    Storage.end();

    TextHandler
        indoorText = TextHandler((String)indoorInt + "°", FontMini),
        outdoorText = TextHandler((String)outdoorInt + "°", FontMini);
    uint8_t
        outdoorHeight = outdoorText.getHeight(),
        marginsY = (ROWS - indoorText.getHeight() - outdoorHeight) / 3;
    Display.clear();
    indoorText.draw((COLUMNS - indoorText.getWidth()) / 2, marginsY);
    outdoorText.draw((COLUMNS - outdoorText.getWidth()) / 2, ROWS - marginsY - outdoorHeight);
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

void HomeThermometerMode::receiverHook(const JsonDocument doc)
{
    if (doc["indoor"].is<float>())
    {
        set("indoor", round(doc["indoor"].as<float>()));
    }
    if (doc["outdoor"].is<float>())
    {
        set("outdoor", round(doc["outdoor"].as<float>()));
    }
}

void HomeThermometerMode::set(const char *const where, const int16_t temperature)
{
    Preferences Storage;
    Storage.begin(std::string(name).substr(0, NVS_KEY_NAME_MAX_SIZE - 1).c_str());
    Storage.putShort(where, temperature);
    Storage.end();
    pending = true;
    transmit();

#ifdef F_DEBUG
    Serial.printf("%s: %s %d\n", name, where, temperature);
#endif
}
