#if EXTENSION_BUTTON

#include "extensions/ButtonExtension.h"

#include "extensions/HomeAssistantExtension.h" // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ModesService.h"

ButtonExtension *Button = nullptr; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

ButtonExtension::ButtonExtension() : ExtensionModule("Button") { Button = this; }

void ButtonExtension::configure()
{
#ifdef PIN_SW1
    pinMode(PIN_SW1, INPUT_PULLUP);
#endif
#ifdef PIN_SW2
    pinMode(PIN_SW2, INPUT_PULLUP);
#endif

#ifdef PIN_SW1
    attachInterrupt(PIN_SW1, &onInterrupt, CHANGE);
#endif
#ifdef PIN_SW2
    attachInterrupt(PIN_SW2, &onInterrupt, CHANGE);
#endif

#if EXTENSION_HOMEASSISTANT
    const std::string topic{std::string("frekvens/" HOSTNAME "/").append(name)};
    {
        for (const char *const payload : {
                 "long",
                 "short",
             })
        {
#ifdef PIN_SW1
            {
                const std::string id{std::string(name).append("_power_").append(payload)};
                JsonObject component{
                    (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
                component[HomeAssistantAbbreviations::automation_type].set("trigger");
                component[HomeAssistantAbbreviations::payload].set(payload);
                component[HomeAssistantAbbreviations::platform].set("device_automation");
                component[HomeAssistantAbbreviations::subtype].set("Power button");
                component[HomeAssistantAbbreviations::topic].set(topic);
                component[HomeAssistantAbbreviations::type].set(
                    std::string("button_").append(payload).append("_press"));
                component[HomeAssistantAbbreviations::value_template].set("{{value_json.event.power}}");
            }
#endif // PIN_SW1
#ifdef PIN_SW2
            {
                const std::string id{std::string(name).append("_mode_").append(payload)};
                JsonObject component{
                    (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
                component[HomeAssistantAbbreviations::automation_type].set("trigger");
                component[HomeAssistantAbbreviations::payload].set(payload);
                component[HomeAssistantAbbreviations::platform].set("device_automation");
                component[HomeAssistantAbbreviations::subtype].set("Mode button");
                component[HomeAssistantAbbreviations::topic].set(topic);
                component[HomeAssistantAbbreviations::type].set(
                    std::string("button_").append(payload).append("_press"));
                component[HomeAssistantAbbreviations::value_template].set("{{value_json.event.mode}}");
            }
#endif // PIN_SW2
        }
    }
#endif // EXTENSION_HOMEASSISTANT
}

void ButtonExtension::handle()
{
#ifdef PIN_SW1
    if (powerShort)
    {
        Display.setPower(!Display.getPower());
        event("power", "short");
        powerShort = false;
    }
    else if (powerState && millis() - powerMillis > UINT8_MAX)
    {
        const uint8_t brightness = Display.getPower() ? Display.getBrightness() : 0;
        if (!powerLong)
        {
            powerLong = true;
            event("power", "long");
            if (brightness >= UINT8_MAX)
            {
                brightnessIncrease = false;
            }
            else if (brightness <= 1)
            {
                brightnessIncrease = true;
            }
            else
            {
                brightnessIncrease = !brightnessIncrease;
            }
        }
        if (brightnessIncrease && brightness < UINT8_MAX)
        {
            Display.setBrightness(brightness + 1);
        }
        else if (!brightnessIncrease && brightness > 1)
        {
            Display.setBrightness(brightness - 1);
        }
    }
#endif // PIN_SW1

#ifdef PIN_SW2
    if (modeShort)
    {
#ifdef PIN_SW1
        Modes.setModeNext();
#else
        ESP_LOGI(name, "power");
        Display.setPower(!Display.getPower());
#endif // PIN_SW1
        event("mode", "short");
        modeShort = false;
    }
    else if (modeState && millis() - modeMillis > (1UL << 10U))
    {
        modeMillis = millis();
        if (!modeLong)
        {
            modeLong = true;
            event("mode", "long");
        }
        Modes.setModeNext();
    }
#endif // PIN_SW2
}

void IRAM_ATTR ButtonExtension::onInterrupt()
{
#ifdef PIN_SW1
    if (digitalRead(PIN_SW1) == LOW)
    {
        Button->powerMillis = millis();
        Button->powerState = true;
    }
    else
    {
        if (Button->powerState && !Button->powerLong)
        {
            Button->powerShort = true;
        }
        Button->powerLong = false;
        Button->powerState = false;
    }
#endif // PIN_SW1
#ifdef PIN_SW2
    if (digitalRead(PIN_SW2) == LOW)
    {
        Button->modeMillis = millis();
        Button->modeState = true;
    }
    else
    {
        if (Button->modeState && !Button->modeLong)
        {
            Button->modeShort = true;
        }
        Button->modeLong = false;
        Button->modeState = false;
    }
#endif // PIN_SW2
}

void ButtonExtension::event(const char *key, const char *value) // NOLINT(bugprone-easily-swappable-parameters)
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["event"][key] = value;
    Device.transmit(doc.as<JsonObjectConst>(), Button->name, false);
}

#endif // EXTENSION_BUTTON
