#include "config/constants.h"

#if EXTENSION_INFRARED

#include <locale/defaults.h>
#include <Preferences.h>

#include "extensions/HomeAssistantExtension.h"
#include "extensions/InfraredExtension.h"
#include "extensions/MicrophoneExtension.h"
#include "extensions/MqttExtension.h"
#include "extensions/PlaylistExtension.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ModesService.h"

InfraredExtension *Infrared = nullptr;

InfraredExtension::InfraredExtension() : ExtensionModule("Infrared")
{
    Infrared = this;
}

void InfraredExtension::setup()
{
    pinMode(PIN_IR, INPUT);
    irrecv = new IRrecv(PIN_IR, kRawBuf, kTimeoutMs, true, kDefaultESP32Timer);

#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_active");
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::command_template] = "{\"active\":{{value}}}";
        component[Abbreviations::command_topic] = topic + "/set";
        component[Abbreviations::entity_category] = "config";
        component[Abbreviations::icon] = "mdi:remote-tv";
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
#endif // EXTENSION_HOMEASSISTANT
}

void InfraredExtension::ready()
{
    Preferences Storage;
    Storage.begin(name, true);
    bool _active = Storage.isKey("active") && Storage.getBool("active");
    Storage.end();
    _active ? set(_active) : transmit();
}

void InfraredExtension::handle()
{
    decode_results results;
    if (irrecv->decode(&results))
    {
#ifdef F_VERBOSE
        if (!results.repeat && results.decode_type != decode_type_t::UNKNOWN && !parse(results))
        {
            switch (results.decode_type)
            {
#if DECODE_RC5
            case decode_type_t::RC5:
                Serial.printf("%s: " D_STR_RC5 " 0x%X\n", name, results.command);
                break;
#endif // DECODE_RC5
#if DECODE_SONY
            case decode_type_t::SONY:
                Serial.printf("%s: " D_STR_SONY " 0x%X\n", name, results.command);
                break;
#endif // DECODE_SONY
            default:
                Serial.printf("%s: protocol %d code 0x%X\n", name, results.decode_type, results.command);
                break;
            }
        }
#else
        if (!results.repeat && results.decode_type != decode_type_t::UNKNOWN)
        {
            parse(results);
        }
#endif // F_VERBOSE
        irrecv->resume();
    }
}

bool InfraredExtension::parse(decode_results results)
{
    for (const Code code : codes)
    {
        if (code.protocol == results.decode_type)
        {
            if (std::find(code.displayBrightnessDecrease.begin(), code.displayBrightnessDecrease.end(), results.command) != code.displayBrightnessDecrease.end())
            {
                if (millis() - lastMillis > INT8_MAX)
                {
                    uint8_t brightness = Display.getGlobalBrightness();
                    if (brightness > 0)
                    {
#ifdef F_INFO
                        Serial.printf("%s: brightness -\n", name);
#endif
                        Display.setGlobalBrightness(max(0, brightness - 5));
                        lastMillis = millis();
                    }
                }
                return true;
            }
            else if (std::find(code.displayBrightnessIncrease.begin(), code.displayBrightnessIncrease.end(), results.command) != code.displayBrightnessIncrease.end())
            {
                if (millis() - lastMillis > INT8_MAX)
                {
                    uint8_t brightness = Display.getGlobalBrightness();
                    if (brightness < UINT8_MAX)
                    {
#ifdef F_INFO
                        Serial.printf("%s: brightness +\n", name);
#endif
                        Display.setGlobalBrightness(min(UINT8_MAX, brightness + 5));
                        lastMillis = millis();
                    }
                }
                return true;
            }
            else if (std::find(code.displayPowerToggle.begin(), code.displayPowerToggle.end(), results.command) != code.displayPowerToggle.end())
            {
                if (millis() - lastMillis > 1000)
                {
#ifdef F_INFO
                    Serial.printf("%s: power\n", name);
#endif
                    Display.setPower(!Display.getPower());
                    lastMillis = millis();
                }
                return true;
            }
#if EXTENSION_MICROPHONE
            else if (std::find(code.extensionMicToggle.begin(), code.extensionMicToggle.end(), results.command) != code.extensionMicToggle.end())
            {
                if (millis() - lastMillis > 1000)
                {
#ifdef F_INFO
                    Serial.printf("%s: microphone\n", name);
#endif
                    Microphone->set(!Microphone->get());
                    lastMillis = millis();
                }
                return true;
            }
#endif // EXTENSION_MICROPHONE
#if EXTENSION_PLAYLIST
            else if (std::find(code.extensionPlaylistToggle.begin(), code.extensionPlaylistToggle.end(), results.command) != code.extensionPlaylistToggle.end())
            {
                if (millis() - lastMillis > 1000)
                {
#ifdef F_INFO
                    Serial.printf("%s: playlist\n", name);
#endif
                    Playlist->set(!Playlist->get());
                    lastMillis = millis();
                }
                return true;
            }
#endif // EXTENSION_PLAYLIST
            else if (std::find(code.modesSetNext.begin(), code.modesSetNext.end(), results.command) != code.modesSetNext.end())
            {
                if (millis() - lastMillis > 500)
                {
#ifdef F_INFO
                    Serial.printf("%s: mode +\n", name);
#endif
                    Modes.next();
                    lastMillis = millis();
                }
                return true;
            }
            else if (std::find(code.modesSetPrevious.begin(), code.modesSetPrevious.end(), results.command) != code.modesSetPrevious.end())
            {
                if (millis() - lastMillis > 500)
                {
#ifdef F_INFO
                    Serial.printf("%s: mode -\n", name);
#endif
                    Modes.previous();
                    lastMillis = millis();
                }
                return true;
            }
            return false;
        }
    }
    return false;
}

bool InfraredExtension::get()
{
    return active;
}

void InfraredExtension::set(bool enable)
{
    if ((enable && !active) || (!enable && active))
    {
        active = enable;
        active ? irrecv->enableIRIn() : irrecv->disableIRIn();

        Preferences Storage;
        Storage.begin(name);
        Storage.putBool("active", active);
        Storage.end();

        transmit();

#ifdef F_INFO
        Serial.printf(active ? "%s: active\n" : "%s: inactive\n", name);
#endif
    }
}

void InfraredExtension::transmit()
{
    JsonDocument doc;
    doc["active"] = active;
    Device.transmit(doc, name);
}

void InfraredExtension::receiverHook(const JsonDocument doc)
{
    // Active
    if (doc["active"].is<bool>())
    {
        set(doc["active"].as<bool>());
    }
}

#endif // EXTENSION_INFRARED
