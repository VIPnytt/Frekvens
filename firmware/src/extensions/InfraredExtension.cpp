#include "config/constants.h"

#if EXTENSION_INFRARED

#include <stdint.h> // temporary bugfix needed for arduino-irremote/IRremote @ 4.5.0
#include <IRremote.hpp>
#include <Preferences.h>

#include "extensions/HomeAssistantExtension.h"
#include "extensions/InfraredExtension.h"
#include "extensions/MicrophoneExtension.h"
#include "extensions/PhotocellExtension.h"
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
    IrReceiver.setReceivePin(PIN_IR);

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
    const bool _active = Storage.isKey("active") && Storage.getBool("active");
    Storage.end();
    _active ? set(true) : transmit();
}

void InfraredExtension::handle()
{
    if (IrReceiver.decode())
    {
        parse();
        IrReceiver.resume();
    }
}

void InfraredExtension::parse()
{
    const unsigned long delta = millis() - lastMillis;
    for (const Code &code : codes)
    {
        if (code.protocol == IrReceiver.decodedIRData.protocol)
        {
            if (delta > INT8_MAX && std::find(code.displayBrightnessDecrease.begin(), code.displayBrightnessDecrease.end(), IrReceiver.decodedIRData.command) != code.displayBrightnessDecrease.end())
            {
#ifdef F_INFO
                Serial.printf("%s: brightness -\n", name);
#endif // F_INFO
                Display.setGlobalBrightness(max(0, Display.getGlobalBrightness() - 5));
                lastMillis = millis();
                return;
            }
            else if (delta > INT8_MAX && std::find(code.displayBrightnessIncrease.begin(), code.displayBrightnessIncrease.end(), IrReceiver.decodedIRData.command) != code.displayBrightnessIncrease.end())
            {
#ifdef F_INFO
                Serial.printf("%s: brightness +\n", name);
#endif // F_INFO
                Display.setGlobalBrightness(min(UINT8_MAX, Display.getGlobalBrightness() + 5));
                lastMillis = millis();
                return;
            }
            else if (delta > (1 << 10) && std::find(code.displayPowerToggle.begin(), code.displayPowerToggle.end(), IrReceiver.decodedIRData.command) != code.displayPowerToggle.end())
            {
#ifdef F_INFO
                Serial.printf("%s: power\n", name);
#endif // F_INFO
                Display.setPower(!Display.getPower());
                lastMillis = millis();
                return;
            }
#if EXTENSION_MICROPHONE
            else if (delta > (1 << 10) && std::find(code.extensionMicrophoneToggle.begin(), code.extensionMicrophoneToggle.end(), IrReceiver.decodedIRData.command) != code.extensionMicrophoneToggle.end())
            {
#ifdef F_INFO
                Serial.printf("%s: microphone\n", name);
#endif // F_INFO
                Microphone->set(!Microphone->get());
                lastMillis = millis();
                return;
            }
#endif // EXTENSION_MICROPHONE
#if EXTENSION_PHOTOCELL
            else if (delta > (1 << 10) && std::find(code.extensionPhotocellToggle.begin(), code.extensionPhotocellToggle.end(), IrReceiver.decodedIRData.command) != code.extensionPhotocellToggle.end())
            {
#ifdef F_INFO
                Serial.printf("%s: photocell\n", name);
#endif // F_INFO
                Photocell->set(!Photocell->get());
                lastMillis = millis();
                return;
            }
#endif // EXTENSION_PHOTOCELL
#if EXTENSION_PLAYLIST
            else if (delta > (1 << 10) && std::find(code.extensionPlaylistToggle.begin(), code.extensionPlaylistToggle.end(), IrReceiver.decodedIRData.command) != code.extensionPlaylistToggle.end())
            {
#ifdef F_INFO
                Serial.printf("%s: playlist\n", name);
#endif // F_INFO
                Playlist->set(!Playlist->get());
                lastMillis = millis();
                return;
            }
#endif // EXTENSION_PLAYLIST
            else if (delta > (1 << 9) && std::find(code.modeNext.begin(), code.modeNext.end(), IrReceiver.decodedIRData.command) != code.modeNext.end())
            {
#ifdef F_INFO
                Serial.printf("%s: mode +\n", name);
#endif // F_INFO
                Modes.next();
                lastMillis = millis();
                return;
            }
            else if (delta > (1 << 9) && std::find(code.modePrevious.begin(), code.modePrevious.end(), IrReceiver.decodedIRData.command) != code.modePrevious.end())
            {
#ifdef F_INFO
                Serial.printf("%s: mode -\n", name);
#endif // F_INFO
                Modes.previous();
                lastMillis = millis();
                return;
            }
            break;
        }
    }
#ifdef F_VERBOSE
    if (!IrReceiver.decodedIRData.flags)
    {
        Serial.printf("%s: %s 0x%X\n", name, ProtocolNames[IrReceiver.decodedIRData.protocol], IrReceiver.decodedIRData.command);
    }
#endif // F_VERBOSE
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
        active ? IrReceiver.start() : IrReceiver.stop();

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
