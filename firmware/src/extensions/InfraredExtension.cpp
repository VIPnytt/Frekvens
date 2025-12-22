#if EXTENSION_INFRARED

#include <stdint.h> // temporary bugfix for arduino-irremote/IRremote @ 4.5.0
#include <IRremote.hpp>
#include <Preferences.h>

#include "config/constants.h"
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

void InfraredExtension::configure()
{
    pinMode(PIN_IR, INPUT);
    IrReceiver.setReceivePin(PIN_IR);

#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_active");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::command_template] = "{\"active\":{{value}}}";
        component[HomeAssistantAbbreviations::command_topic] = topic + "/set";
        component[HomeAssistantAbbreviations::entity_category] = "config";
        component[HomeAssistantAbbreviations::icon] = "mdi:remote-tv";
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
#endif // EXTENSION_HOMEASSISTANT
}

void InfraredExtension::begin()
{
    Preferences Storage;
    Storage.begin(name, true);
    const bool _active = Storage.isKey("active") && Storage.getBool("active");
    Storage.end();
    _active ? setActive(true) : transmit();
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
    const unsigned long t = millis() - lastMillis;
    for (const Code &code : codes)
    {
        if (code.protocol == IrReceiver.decodedIRData.protocol)
        {
            if (t > INT8_MAX && std::find(code.displayBrightnessDecrease.begin(), code.displayBrightnessDecrease.end(), IrReceiver.decodedIRData.command) != code.displayBrightnessDecrease.end())
            {
                Display.setBrightness(max(1, Display.getBrightness() - 5));
                lastMillis = millis();
                return;
            }
            else if (t > INT8_MAX && std::find(code.displayBrightnessIncrease.begin(), code.displayBrightnessIncrease.end(), IrReceiver.decodedIRData.command) != code.displayBrightnessIncrease.end())
            {
                Display.setBrightness(min(UINT8_MAX, Display.getBrightness() + 5));
                lastMillis = millis();
                return;
            }
            else if (t > (1 << 10) && std::find(code.displayPowerToggle.begin(), code.displayPowerToggle.end(), IrReceiver.decodedIRData.command) != code.displayPowerToggle.end())
            {
                Display.setPower(!Display.getPower());
                lastMillis = millis();
                return;
            }
#if EXTENSION_MICROPHONE
            else if (t > (1 << 10) && std::find(code.extensionMicrophoneToggle.begin(), code.extensionMicrophoneToggle.end(), IrReceiver.decodedIRData.command) != code.extensionMicrophoneToggle.end())
            {
                Microphone->setActive(!Microphone->getActive());
                lastMillis = millis();
                return;
            }
#endif // EXTENSION_MICROPHONE
#if EXTENSION_PHOTOCELL
            else if (t > (1 << 10) && std::find(code.extensionPhotocellToggle.begin(), code.extensionPhotocellToggle.end(), IrReceiver.decodedIRData.command) != code.extensionPhotocellToggle.end())
            {
                Photocell->setActive(!Photocell->getActive());
                lastMillis = millis();
                return;
            }
#endif // EXTENSION_PHOTOCELL
#if EXTENSION_PLAYLIST
            else if (t > (1 << 10) && std::find(code.extensionPlaylistStart.begin(), code.extensionPlaylistStart.end(), IrReceiver.decodedIRData.command) != code.extensionPlaylistStart.end())
            {
                Playlist->setActive(true);
                lastMillis = millis();
                return;
            }
            else if (t > (1 << 10) && std::find(code.extensionPlaylistStop.begin(), code.extensionPlaylistStop.end(), IrReceiver.decodedIRData.command) != code.extensionPlaylistStop.end())
            {
                Playlist->setActive(false);
                lastMillis = millis();
                return;
            }
#endif // EXTENSION_PLAYLIST
            else if (t > (1 << 9) && std::find(code.modeNext.begin(), code.modeNext.end(), IrReceiver.decodedIRData.command) != code.modeNext.end())
            {
                Modes.setModeNext();
                lastMillis = millis();
                return;
            }
            else if (t > (1 << 9) && std::find(code.modePrevious.begin(), code.modePrevious.end(), IrReceiver.decodedIRData.command) != code.modePrevious.end())
            {
                Modes.setModePrevious();
                lastMillis = millis();
                return;
            }
            break;
        }
    }
    if (!IrReceiver.decodedIRData.flags)
    {
        ESP_LOGV(name, "%s 0x%X", ProtocolNames[IrReceiver.decodedIRData.protocol], IrReceiver.decodedIRData.command);
    }
}

bool InfraredExtension::getActive()
{
    return active;
}

void InfraredExtension::setActive(bool active)
{
    if ((active && !this->active) || (!active && this->active))
    {
        this->active = active;
        this->active ? IrReceiver.start() : IrReceiver.stop();

        Preferences Storage;
        Storage.begin(name);
        Storage.putBool("active", this->active);
        Storage.end();

        transmit();

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

void InfraredExtension::transmit()
{
    JsonDocument doc;
    doc["active"] = active;
    Device.transmit(doc, name);
}

void InfraredExtension::onReceive(const JsonDocument doc, const char *const source)
{
    // Active
    if (doc["active"].is<bool>())
    {
        setActive(doc["active"].as<bool>());
    }
}

#endif // EXTENSION_INFRARED
