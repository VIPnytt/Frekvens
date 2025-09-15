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
        const std::string id = std::string(name).append("_gamma");
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::command_template] = "{\"gamma\":{{value}}}";
        component[Abbreviations::command_topic] = topic + "/set";
        component[Abbreviations::enabled_by_default] = false;
        component[Abbreviations::entity_category] = "config";
        component[Abbreviations::icon] = "mdi:brightness-auto";
        component[Abbreviations::max] = 10;
        component[Abbreviations::min] = 0.1f;
        component[Abbreviations::mode] = "box";
        component[Abbreviations::name] = "Gamma";
        component[Abbreviations::object_id] = HOSTNAME "_" + id;
        component[Abbreviations::platform] = "number";
        component[Abbreviations::state_topic] = topic;
        component[Abbreviations::step] = 0.1f;
        component[Abbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[Abbreviations::value_template] = "{{value_json.gamma}}";
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
    {
        const std::string id = std::string(name).append("_threshold");
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::command_template] = "{\"threshold\":{{value}}}";
        component[Abbreviations::command_topic] = topic + "/set";
        component[Abbreviations::enabled_by_default] = false;
        component[Abbreviations::entity_category] = "config";
        component[Abbreviations::icon] = "mdi:brightness-auto";
        component[Abbreviations::max] = (1 << 7) - 1;
        component[Abbreviations::mode] = "slider";
        component[Abbreviations::name] = "Threshold";
        component[Abbreviations::object_id] = HOSTNAME "_" + id;
        component[Abbreviations::platform] = "number";
        component[Abbreviations::state_topic] = topic;
        component[Abbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[Abbreviations::value_template] = "{{value_json.threshold}}";
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
    if (Storage.isKey("max"))
    {
        levelMax = Storage.getUShort("max");
    }
    if (Storage.isKey("min"))
    {
        levelMin = Storage.getUShort("min");
    }
    if (Storage.isKey("threshold"))
    {
        threshold = Storage.getUShort("threshold");
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
            read();
        }
        transmit();
    }
    else if (active && Display.getPower() && millis() - _lastMillis > UINT8_MAX)
    {
        _lastMillis = millis();
        read();
        const uint8_t _brightness = round(powf((level - levelMin) / (float)(levelMax - levelMin), gamma) * UINT8_MAX);
        if (_brightness > brightness)
        {
            if (debounce >= threshold)
            {
                setBrightness(_brightness);
            }
            else
            {
                debounce += 1;
            }
        }
        else if (_brightness < brightness)
        {
            if (debounce <= -threshold)
            {
                setBrightness(_brightness);
            }
            else
            {
                debounce -= 1;
            }
        }
    }
}

void PhotocellExtension::read()
{
    level = analogReadRaw(PIN_LDR);
    if (level > levelMax)
    {
        levelMax = level;
        Preferences Storage;
        Storage.begin(name);
        Storage.putUShort("max", levelMax);
        Storage.end();
        pending = true;
    }
    else if (level < levelMin)
    {
        levelMin = level;
        Preferences Storage;
        Storage.begin(name);
        Storage.putUShort("min", levelMin);
        Storage.end();
        pending = true;
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
            debounce = 0;
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

void PhotocellExtension::setBrightness(uint8_t _brightness)
{
    brightness = _brightness;
    debounce = 0;
#ifdef F_INFO
    Serial.printf("%s: brightness\n", name);
#endif // F_INFO
    Display.setGlobalBrightness(brightness);
}

void PhotocellExtension::setGamma(float _gamma)
{
    if (_gamma != gamma && _gamma > 0)
    {
        gamma = _gamma;
        Preferences Storage;
        Storage.begin(name);
        Storage.putFloat("gamma", gamma);
        Storage.end();
        pending = true;
    }
}

void PhotocellExtension::setThreshold(int8_t _threshold)
{
    if (_threshold != threshold && _threshold >= 0)
    {
        threshold = _threshold;
        Preferences Storage;
        Storage.begin(name);
        Storage.putUShort("threshold", threshold);
        Storage.end();
        pending = true;
    }
}

void PhotocellExtension::reset()
{
    levelMax = level;
    levelMin = level;
}

void PhotocellExtension::transmit()
{
    JsonDocument doc;
    doc["active"] = active;
    doc["gamma"] = gamma;
    doc["illuminance"] = level;
    doc["threshold"] = threshold;
    Device.transmit(doc, name);
    lastMillis = millis();
}

void PhotocellExtension::receiverHook(const JsonDocument doc)
{
    // Action: Reset
    if (doc["action"].is<const char *>() && !strcmp(doc["action"].as<const char *>(), "reset"))
    {
        reset();
    }
    // Active
    if (doc["active"].is<bool>())
    {
        set(doc["active"].as<bool>());
    }
    // Gamma
    if (doc["gamma"].is<float>())
    {
        setGamma(doc["gamma"].as<float>());
    }
    // Threshold
    if (doc["threshold"].is<int8_t>())
    {
        setThreshold(doc["threshold"].as<int8_t>());
    }
}

void PhotocellExtension::transmitterHook(const JsonDocument &doc, const char *const source)
{
    // Display: Brightness
    if (!strcmp(source, Display.name) && doc["brightness"].is<uint8_t>() && doc["brightness"].as<uint8_t>() != brightness)
    {
        set(false);
    }
}

#endif // EXTENSION_PHOTOCELL
