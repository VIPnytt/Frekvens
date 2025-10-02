#include "config/constants.h"

#if EXTENSION_BUTTON

#include "extensions/ButtonExtension.h"
#include "extensions/HomeAssistantExtension.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ModesService.h"

ButtonExtension *Button = nullptr;

ButtonExtension::ButtonExtension() : ExtensionModule("Button")
{
    Button = this;
}

void ButtonExtension::setup()
{
#ifdef PIN_SW1
    pinMode(PIN_SW1, INPUT_PULLUP);
#endif
#ifdef PIN_SW2
    pinMode(PIN_SW2, INPUT_PULLUP);
#endif

#ifdef PIN_SW1
    attachInterrupt(digitalPinToInterrupt(PIN_SW1), &onInterrupt, CHANGE);
#endif
#ifdef PIN_SW2
    attachInterrupt(digitalPinToInterrupt(PIN_SW2), &onInterrupt, CHANGE);
#endif

#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        for (const char *const payload : {
                 "long",
                 "short",
             })
        {
#ifdef PIN_SW1
            {
                const std::string id = std::string(name).append("_power_").append(payload);
                JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
                component[Abbreviations::automation_type] = "trigger";
                component[Abbreviations::payload] = payload;
                component[Abbreviations::platform] = "device_automation";
                component[Abbreviations::subtype] = "Power button";
                component[Abbreviations::topic] = topic;
                component[Abbreviations::type] = std::string("button_").append(payload).append("_press");
                component[Abbreviations::value_template] = "{{value_json.event.power}}";
            }
#endif // PIN_SW1
#ifdef PIN_SW2
            {
                const std::string id = std::string(name).append("_mode_").append(payload);
                JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
                component[Abbreviations::automation_type] = "trigger";
                component[Abbreviations::payload] = payload;
                component[Abbreviations::platform] = "device_automation";
                component[Abbreviations::subtype] = "Mode button";
                component[Abbreviations::topic] = topic;
                component[Abbreviations::type] = std::string("button_").append(payload).append("_press");
                component[Abbreviations::value_template] = "{{value_json.event.mode}}";
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
#ifdef F_INFO
        Serial.printf("%s: power\n", Button->name);
#endif // F_INFO
        Display.setPower(!Display.getPower());
        event("power", "short");
        powerShort = false;
    }
    else if (powerState && millis() - powerMillis > UINT8_MAX)
    {
        const uint8_t brightness = Display.getGlobalBrightness();
        if (!powerLong)
        {
            powerLong = true;
            event("power", "long");
            switch (brightness)
            {
            case 0:
                brightnessIncrease = true;
                break;
            case UINT8_MAX:
                brightnessIncrease = false;
                break;
            default:
                brightnessIncrease = !brightnessIncrease;
            }
        }
        if (brightnessIncrease && brightness < UINT8_MAX)
        {
#ifdef F_INFO
            Serial.printf("%s: brightness +\n", Button->name);
#endif // F_INFO
            Display.setGlobalBrightness(brightness + 1);
        }
        else if (!brightnessIncrease && brightness > 0)
        {
#ifdef F_INFO
            Serial.printf("%s: brightness -\n", Button->name);
#endif // F_INFO
            Display.setGlobalBrightness(brightness - 1);
        }
    }
#endif // PIN_SW1

#ifdef PIN_SW2
    if (modeShort)
    {
#ifdef PIN_SW1
#ifdef F_INFO
        Serial.printf("%s: mode\n", Button->name);
#endif // F_INFO
        Modes.next();
#else
#ifdef F_INFO
        Serial.printf("%s: power\n", Button->name);
#endif // F_INFO
        Display.setPower(!Display.getPower());
#endif // PIN_SW1
        event("mode", "short");
        modeShort = false;
    }
    else if (modeState && millis() - modeMillis > 1000)
    {
        modeMillis = millis();
        if (!modeLong)
        {
            modeLong = true;
            event("mode", "long");
        }
#ifdef F_INFO
        Serial.printf("%s: mode\n", Button->name);
#endif // F_INFO
        Modes.next();
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

void ButtonExtension::event(const char *key, const char *value)
{
    JsonDocument doc;
    doc["event"][key] = value;
    Device.transmit(doc, Button->name, false);
}

#endif // EXTENSION_BUTTON
