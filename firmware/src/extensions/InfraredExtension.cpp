#if EXTENSION_INFRARED

#include "extensions/InfraredExtension.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/DisplayService.h" // NOLINT(misc-include-cleaner)
#include "services/ExtensionsService.h"
#include "services/ModesService.h" // NOLINT(misc-include-cleaner)

#include <IRremote.hpp> // NOLINT(misc-include-cleaner)
#include <Preferences.h>

void InfraredExtension::configure()
{
    pinMode(PIN_IR, INPUT);
    IrReceiver.setReceivePin(PIN_IR);
}

void InfraredExtension::begin()
{
    Preferences Storage;
    Storage.begin(name.data(), true);
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

void InfraredExtension::parse() // NOLINT(readability-make-member-function-const)
{
    const unsigned long delta = millis() - lastMillis;
    for (const Code &code : codes)
    {
        if (code.protocol == IrReceiver.decodedIRData.protocol)
        {
            if (delta > INT8_MAX && std::find(code.displayBrightnessDecrease.begin(),
                                              code.displayBrightnessDecrease.end(),
                                              IrReceiver.decodedIRData.command) != code.displayBrightnessDecrease.end())
            {
                Display.setBrightness(max(1, Display.getBrightness() - 5));
                lastMillis = millis();
                return;
            }
            else if (delta > INT8_MAX &&
                     std::find(code.displayBrightnessIncrease.begin(),
                               code.displayBrightnessIncrease.end(),
                               IrReceiver.decodedIRData.command) != code.displayBrightnessIncrease.end())
            {
                Display.setBrightness(min(UINT8_MAX, Display.getBrightness() + 5));
                lastMillis = millis();
                return;
            }
            else if (delta > (1UL << 10U) &&
                     std::find(code.displayPowerToggle.begin(),
                               code.displayPowerToggle.end(),
                               IrReceiver.decodedIRData.command) != code.displayPowerToggle.end())
            {
                Display.setPower(!Display.getPower());
                lastMillis = millis();
                return;
            }
#if EXTENSION_MICROPHONE
            else if (delta > (1UL << 10U) &&
                     std::find(code.extensionMicrophoneToggle.begin(),
                               code.extensionMicrophoneToggle.end(),
                               IrReceiver.decodedIRData.command) != code.extensionMicrophoneToggle.end())
            {
                Extensions.Microphone().setActive(!Extensions.Microphone().getActive());
                lastMillis = millis();
                return;
            }
#endif // EXTENSION_MICROPHONE
#if EXTENSION_PHOTOCELL
            else if (delta > (1UL << 10U) &&
                     std::find(code.extensionPhotocellToggle.begin(),
                               code.extensionPhotocellToggle.end(),
                               IrReceiver.decodedIRData.command) != code.extensionPhotocellToggle.end())
            {
                Extensions.Photocell().setActive(!Extensions.Photocell().getActive());
                lastMillis = millis();
                return;
            }
#endif // EXTENSION_PHOTOCELL
#if EXTENSION_PLAYLIST
            else if (delta > (1UL << 10U) &&
                     std::find(code.extensionPlaylistStart.begin(),
                               code.extensionPlaylistStart.end(),
                               IrReceiver.decodedIRData.command) != code.extensionPlaylistStart.end())
            {
                Extensions.Playlist().setActive(true);
                lastMillis = millis();
                return;
            }
            else if (delta > (1UL << 10U) &&
                     std::find(code.extensionPlaylistStop.begin(),
                               code.extensionPlaylistStop.end(),
                               IrReceiver.decodedIRData.command) != code.extensionPlaylistStop.end())
            {
                Extensions.Playlist().setActive(false);
                lastMillis = millis();
                return;
            }
#endif // EXTENSION_PLAYLIST
            else if (delta > (1UL << 9U) &&
                     std::find(code.modeNext.begin(), code.modeNext.end(), IrReceiver.decodedIRData.command) !=
                         code.modeNext.end())
            {
                Modes.setModeNext();
                lastMillis = millis();
                return;
            }
            else if (delta > (1UL << 9U) &&
                     std::find(code.modePrevious.begin(), code.modePrevious.end(), IrReceiver.decodedIRData.command) !=
                         code.modePrevious.end())
            {
                Modes.setModePrevious();
                lastMillis = millis();
                return;
            }
            break;
        }
    }
    if (IrReceiver.decodedIRData.flags == 0)
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        ESP_LOGV(name, "%s 0x%X", ProtocolNames[IrReceiver.decodedIRData.protocol], IrReceiver.decodedIRData.command);
    }
}

bool InfraredExtension::getActive() const { return active; }

void InfraredExtension::setActive(bool active)
{
    if ((active && !this->active) || (!active && this->active))
    {
        this->active = active;
        this->active ? IrReceiver.start() : IrReceiver.stop();
        Preferences Storage;
        Storage.begin(name.data());
        Storage.putBool("active", this->active);
        Storage.end();
        transmit();
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        ESP_LOGI(name, "%s", this->active ? "active" : "inactive");
    }
}

void InfraredExtension::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["active"].set(active);
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void InfraredExtension::onReceive(JsonObjectConst payload,
                                  std::string_view source) // NOLINT(misc-unused-parameters)
{
    // Active
    if (payload["active"].is<bool>())
    {
        setActive(payload["active"].as<bool>());
    }
}

#if EXTENSION_HOMEASSISTANT
void InfraredExtension::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique)
{
    topic.append(name);
    {
        const std::string id{std::string(name).append("_active")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"active":{{value}}})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::entity_category].set("config");
        component[HomeAssistantAbbreviations::icon].set("mdi:remote-tv");
        component[HomeAssistantAbbreviations::name].set(name);
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        component[HomeAssistantAbbreviations::payload_off].set("false");
        component[HomeAssistantAbbreviations::payload_on].set("true");
        component[HomeAssistantAbbreviations::platform].set("switch");
        component[HomeAssistantAbbreviations::state_off].set("False");
        component[HomeAssistantAbbreviations::state_on].set("True");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.active}}");
    }
}
#endif // EXTENSION_HOMEASSISTANT

#endif // EXTENSION_INFRARED
