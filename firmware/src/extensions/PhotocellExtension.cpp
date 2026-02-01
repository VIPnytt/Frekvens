#if EXTENSION_PHOTOCELL

#include "extensions/PhotocellExtension.h"

#include "config/constants.h"
#include "extensions/HomeAssistantExtension.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"

#include <Preferences.h>

PhotocellExtension *Photocell = nullptr;

PhotocellExtension::PhotocellExtension() : ExtensionModule("Photocell") { Photocell = this; }

void PhotocellExtension::configure()
{
    pinMode(PIN_LDR, ANALOG);
#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_active");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::command_template] = "{\"active\":{{value}}}";
        component[HomeAssistantAbbreviations::command_topic] = topic + "/set";
        component[HomeAssistantAbbreviations::icon] = "mdi:brightness-auto";
        component[HomeAssistantAbbreviations::name] = name;
        component[HomeAssistantAbbreviations::object_id] = HOSTNAME "_" + id;
        component[HomeAssistantAbbreviations::payload_off] = "false";
        component[HomeAssistantAbbreviations::payload_on] = "true";
        component[HomeAssistantAbbreviations::platform] = "switch";
        component[HomeAssistantAbbreviations::state_off] = "False";
        component[HomeAssistantAbbreviations::state_on] = "True";
        component[HomeAssistantAbbreviations::state_topic] = topic;
        component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[HomeAssistantAbbreviations::value_template] = "{{value_json.active}}";
    }
    {
        const std::string id = std::string(name).append("_illuminance");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::enabled_by_default] = false;
        component[HomeAssistantAbbreviations::entity_category] = "diagnostic";
        component[HomeAssistantAbbreviations::icon] = "mdi:brightness-5";
        component[HomeAssistantAbbreviations::name] = "Illuminance";
        component[HomeAssistantAbbreviations::object_id] = HOSTNAME "_" + id;
        component[HomeAssistantAbbreviations::platform] = "sensor";
        component[HomeAssistantAbbreviations::state_class] = "measurement";
        component[HomeAssistantAbbreviations::state_topic] = topic;
        component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[HomeAssistantAbbreviations::value_template] = "{{value_json.illuminance}}";
    }
#endif // EXTENSION_HOMEASSISTANT
}

void PhotocellExtension::begin()
{
    Preferences Storage;
    Storage.begin(name, true);
    const bool _active = Storage.isKey("active") && Storage.getBool("active");
    if (Storage.isKey("gamma"))
    {
        gamma = Storage.getFloat("gamma");
    }
    Storage.end();
    _active ? setActive(true) : transmit();
}

void PhotocellExtension::handle()
{
    if (pending || millis() - lastMillis > UINT16_MAX)
    {
        pending = false;
        if (!active || !Display.getPower())
        {
            raw = analogRead(PIN_LDR);
        }
        transmit();
    }
    else if (active && Display.getPower() && millis() - _lastMillis > UINT8_MAX)
    {
        _lastMillis = millis();
        raw = analogRead(PIN_LDR);
        const uint8_t _brightness = pow((raw) / (float)((1 << 12) - 1), gamma) * UINT8_MAX;
        if ((direction && _brightness < brightness) || (!direction && _brightness > brightness))
        {
            direction = !direction;
            counter /= 2;
        }
        counter += _brightness - brightness;
        if (abs(counter) > UINT8_MAX)
        {
            brightness = _brightness;
            counter = 0;
            Display.setBrightness(brightness);
        }
    }
}

bool PhotocellExtension::getActive() const { return active; }

void PhotocellExtension::setActive(bool active)
{
    if ((active && !this->active) || (!active && this->active))
    {
        if (active)
        {
            counter = 0;
            brightness = Display.getBrightness();
        }
        this->active = active;
        Preferences Storage;
        Storage.begin(name);
        Storage.putBool("active", this->active);
        Storage.end();
        pending = true;
        if (this->active)
        {
            ESP_LOGI(name, "active");
        }
        else
        {
            ESP_LOGI(name, "inactive");
        }
    }
}

void PhotocellExtension::setGamma(float _gamma)
{
    if (_gamma != gamma)
    {
        gamma = _gamma;
        Preferences Storage;
        Storage.begin(name);
        Storage.putFloat("gamma", gamma);
        Storage.end();
    }
}

void PhotocellExtension::transmit()
{
    JsonDocument doc;
    doc["active"] = active;
    doc["illuminance"] = raw;
    Device.transmit(doc, name);
    lastMillis = millis();
}

void PhotocellExtension::onReceive(const JsonDocument doc, const char *const source)
{
    // Active
    if (doc["active"].is<bool>())
    {
        setActive(doc["active"].as<bool>());
    }
}

void PhotocellExtension::onTransmit(const JsonDocument &doc, const char *const source)
{
    // Display: Brightness
    if (active && !strcmp(source, Display.name) && doc["brightness"].is<uint8_t>())
    {
        const uint8_t _brightness = doc["brightness"].as<uint8_t>();
        if (_brightness != brightness)
        {
            setGamma(log(_brightness / (float)(1 << 8)) / log((raw + 1) / (float)((1 << 12) + 1)));
        }
    }
}

#endif // EXTENSION_PHOTOCELL
