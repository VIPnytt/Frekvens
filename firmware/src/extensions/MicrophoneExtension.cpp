#include "config/constants.h"

#if EXTENSION_MICROPHONE

#include <Preferences.h>

#include "extensions/BuildExtension.h"
#include "extensions/HomeAssistantExtension.h"
#include "extensions/MicrophoneExtension.h"
#include "extensions/MqttExtension.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"

MicrophoneExtension *Microphone = nullptr;

MicrophoneExtension::MicrophoneExtension() : ExtensionModule("Microphone")
{
    Microphone = this;
}

void MicrophoneExtension::setup()
{
    pinMode(PIN_MIC, ANALOG);

#if EXTENSION_BUILD && defined(MIC_FLOOR)
    (*Build->config)[Config::h][__STRING(MIC_FLOOR)] = MIC_FLOOR;
#endif // EXTENSION_BUILD && defined(MIC_FLOOR)

#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_active");
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::command_template] = "{\"active\":{{value}}}";
        component[Abbreviations::command_topic] = topic + "/set";
        component[Abbreviations::icon] = "mdi:microphone";
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
        const std::string id = std::string(name).append("_level");
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::enabled_by_default] = false;
        component[Abbreviations::entity_category] = "diagnostic";
        component[Abbreviations::expire_after] = INT8_MAX;
        component[Abbreviations::force_update] = true;
        component[Abbreviations::icon] = "mdi:microphone";
        component[Abbreviations::name] = "Sound level";
        component[Abbreviations::object_id] = HOSTNAME "_" + id;
        component[Abbreviations::platform] = "sensor";
        component[Abbreviations::state_class] = "measurement";
        component[Abbreviations::state_topic] = topic;
        component[Abbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[Abbreviations::value_template] = "{{value_json.level}}";
    }
#endif // EXTENSION_HOMEASSISTANT
}

void MicrophoneExtension::ready()
{
    Preferences Storage;
    Storage.begin(name, true);
    const bool _active = Storage.isKey("active") && Storage.getBool("active");
    Storage.end();
    _active ? set(true) : transmit();
}

void MicrophoneExtension::handle()
{
    if (pending || (active && Display.getPower() && millis() - lastMillis > UINT16_MAX))
    {
        pending = false;
        lastMillis = millis();
        transmit();
        level = 0;
    }
    else if (active && Display.getPower())
    {
        const uint16_t lastMic = mic;
        mic = analogReadRaw(PIN_MIC);
        uint16_t _level = abs(mic - lastMic);
        if (_level > level)
        {
            level = _level;
        }
        if (_level > floor)
        {
            _lastMillis = millis();
            if (!detected)
            {
                detected = true;
#ifdef F_VERBOSE
                Serial.print(name);
                Serial.print(": sound, level ");
                Serial.println(level);
#endif
            }
        }
        else if (detected && millis() - _lastMillis > INT8_MAX)
        {
            detected = false;
#ifdef F_VERBOSE
            Serial.print(name);
            Serial.print(": silence, level ");
            Serial.println(level);
#endif
        }
    }
}

bool MicrophoneExtension::get()
{
    return active;
}

void MicrophoneExtension::set(bool enable)
{
    if ((enable && !active) || (!enable && active))
    {
        active = enable;

        Preferences Storage;
        Storage.begin(name);
        Storage.putBool("active", active);
        Storage.end();

        if (active)
        {
            mic = analogReadRaw(PIN_MIC);
        }
        pending = true;

#ifdef F_INFO
        Serial.print(name);
        Serial.println(active ? ": active" : ": inactive");
#endif
    }
}

bool MicrophoneExtension::play()
{
    return detected || !active;
}

void MicrophoneExtension::transmit()
{
    JsonDocument doc;
    doc["active"] = active;
    doc["floor"] = floor;
    doc["level"] = level;
    Device.transmit(doc, name);
}

void MicrophoneExtension::receiverHook(const JsonDocument doc)
{
    // Active
    if (doc["active"].is<bool>())
    {
        set(doc["active"].as<bool>());
    }
}

#endif // EXTENSION_MICROPHONE
