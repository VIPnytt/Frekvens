#include "config/constants.h"

#if EXTENSION_PHOTOCELL

#include <Preferences.h>

#include "extensions/HomeAssistantExtension.h"
#include "extensions/PhotocellExtension.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"

PhotocellExtension *Photocell = nullptr;

PhotocellExtension::PhotocellExtension() : ExtensionModule("Photocell")
{
    Photocell = this;
}

void PhotocellExtension::setup()
{
    pinMode(PIN_LDR, ANALOG);
#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_active");
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::command_template] = "{\"active\":{{value}}}";
        component[Abbreviations::command_topic] = topic + "/set";
        component[Abbreviations::icon] = "mdi:brightness-auto";
        component[Abbreviations::name] = name;
        component[Abbreviations::object_id] = HOSTNAME "_" + id;
        component[Abbreviations::payload_off] = "false";
        component[Abbreviations::payload_on] = "true";
        component[Abbreviations::platform] = "switch";
        component[Abbreviations::state_off] = "False";
        component[Abbreviations::state_on] = "True";
        component[Abbreviations::state_topic] = topic;
        component[Abbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[Abbreviations::value_template] = "{{value_json.active}}";
    }
    {
        const std::string id = std::string(name).append("_illuminance");
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::enabled_by_default] = false;
        component[Abbreviations::entity_category] = "diagnostic";
        component[Abbreviations::icon] = "mdi:brightness-5";
        component[Abbreviations::name] = "Illuminance";
        component[Abbreviations::object_id] = HOSTNAME "_" + id;
        component[Abbreviations::platform] = "sensor";
        component[Abbreviations::state_class] = "measurement";
        component[Abbreviations::state_topic] = topic;
        component[Abbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[Abbreviations::value_template] = "{{value_json.illuminance}}";
    }
#endif // EXTENSION_HOMEASSISTANT
}

void PhotocellExtension::ready()
{
    Preferences Storage;
    Storage.begin(name, true);
    const bool _active = Storage.isKey("active") && Storage.getBool("active");
    if (Storage.isKey("gamma"))
    {
        gamma = Storage.getFloat("gamma");
    }
    Storage.end();
    _active ? set(true) : transmit();
}

void PhotocellExtension::handle()
{
    if (pending || millis() - lastMillis > UINT16_MAX)
    {
        pending = false;
        if (!active || !Display.getPower())
        {
            raw = analogReadRaw(PIN_LDR);
        }
        transmit();
    }
    else if (active && Display.getPower() && millis() - _lastMillis > UINT8_MAX)
    {
        _lastMillis = millis();
        raw = analogReadRaw(PIN_LDR);
        const uint8_t _brightness = round(powf((raw) / (float)((1 << 12) - 1), gamma) * UINT8_MAX);
        if ((direction && _brightness < brightness) || (!direction && _brightness > brightness))
        {
            direction = !direction;
            counter /= 2;
        }
        counter += _brightness - brightness;
        if (counter <= -threshold || counter >= threshold)
        {
            brightness = _brightness;
            counter = 0;
#ifdef F_INFO
            Serial.printf("%s: brightness\n", name);
#endif // F_INFO
            Display.setGlobalBrightness(brightness);
        }
    }
}

bool PhotocellExtension::get()
{
    return active;
}

void PhotocellExtension::set(bool enable)
{
    if ((enable && !active) || (!enable && active))
    {
        if (enable)
        {
            counter = 0;
            brightness = Display.getGlobalBrightness();
        }
        active = enable;
        Preferences Storage;
        Storage.begin(name);
        Storage.putBool("active", active);
        Storage.end();
        pending = true;
#ifdef F_INFO
        Serial.printf(active ? "%s: active\n" : "%s: inactive\n", name);
#endif
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

void PhotocellExtension::receiverHook(const JsonDocument doc)
{
    // Active
    if (doc["active"].is<bool>())
    {
        set(doc["active"].as<bool>());
    }
}

void PhotocellExtension::transmitterHook(const JsonDocument &doc, const char *const source)
{
    // Display: Brightness
    if (active && !strcmp(source, Display.name) && doc["brightness"].is<uint8_t>())
    {
        const uint8_t _brightness = doc["brightness"].as<uint8_t>();
        if (_brightness != brightness)
        {
            setGamma(log((_brightness + 1) / (double)((1 << 8) + 1)) / log((raw + 1) / (double)((1 << 12) + 1)));
        }
    }
}

#endif // EXTENSION_PHOTOCELL
