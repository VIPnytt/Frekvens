#if EXTENSION_BUTTON

#include "extensions/ButtonExtension.h"

#include "extensions/HomeAssistantExtension.h" // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ModesService.h"

void ButtonExtension::configure()
{
#ifdef PIN_SW1
    pinMode(PIN_SW1, INPUT_PULLUP);
#endif
#ifdef PIN_SW2
    pinMode(PIN_SW2, INPUT_PULLUP);
#endif
#ifdef PIN_SW1
    attachInterrupt(PIN_SW1, &onChangePower, CHANGE);
#endif
#ifdef PIN_SW2
    attachInterrupt(PIN_SW2, &onChangeMode, CHANGE);
#endif
}

void ButtonExtension::handle()
{
#ifdef PIN_SW1
    if (powerState == State::SHORT)
    {
        powerState = State::IDLE;
        Display.setPower(!Display.getPower());
        event("power", "short");
    }
    else if (powerState == State::PRESS && millis() - powerMillis > UINT8_MAX)
    {
        powerState = State::LONG;
        event("power", "long");
        if (!Display.getPower())
        {
            powerMillis = millis();
            brightnessIncrease = true;
            Display.setBrightness(1U);
        }
        else
        {
            const uint8_t brightness{Display.getBrightness()};
            if (brightness == UINT8_MAX)
            {
                brightnessIncrease = false;
            }
            else if (brightness <= 1U)
            {
                brightnessIncrease = true;
            }
            else
            {
                brightnessIncrease = !brightnessIncrease;
            }
        }
    }
    else if (powerState == State::LONG && millis() - powerMillis > (0b1U << 4U))
    {
        powerMillis = millis();
        const uint8_t brightness{Display.getBrightness()};
        if (brightnessIncrease && brightness < UINT8_MAX)
        {
            Display.setBrightness(brightness + 1U);
        }
        else if (!brightnessIncrease && brightness > 1U)
        {
            Display.setBrightness(brightness - 1U);
        }
    }
#endif // PIN_SW1
#ifdef PIN_SW2
    if (modeState == State::SHORT)
    {
        modeState = State::IDLE;
#ifdef PIN_SW1
        Modes.setModeNext();
#else
        ESP_LOGI(name.data(), "power");
        Display.setPower(!Display.getPower());
#endif // PIN_SW1
        event("mode", "short");
    }
    else if (modeState == State::PRESS && millis() - modeMillis > UINT8_MAX)
    {
        modeState = State::LONG;
        event("mode", "long");
        if (!Display.getPower())
        {
            modeMillis = millis();
            Display.setPower(true);
        }
    }
    else if (modeState == State::LONG && millis() - modeMillis > (0b1U << 10U))
    {
        modeMillis = millis();
        Modes.setModeNext();
    }
#endif // PIN_SW2
}

#ifdef PIN_SW1
void IRAM_ATTR ButtonExtension::onChangePower()
{
    if (digitalRead(PIN_SW1) == LOW)
    {
        powerMillis = millis();
        powerState = State::PRESS;
    }
    else
    {
        powerState = powerState == State::PRESS ? State::SHORT : State::IDLE;
    }
}
#endif // PIN_SW1

#ifdef PIN_SW2
void IRAM_ATTR ButtonExtension::onChangeMode()
{
    if (digitalRead(PIN_SW2) == LOW)
    {
        modeMillis = millis();
        modeState = State::PRESS;
    }
    else
    {
        modeState = modeState == State::PRESS ? State::SHORT : State::IDLE;
    }
}
#endif // PIN_SW2

/**
 * @brief Transmits a button event with the specified key and value.
 *
 * @param key Event field name.
 * @param value Event field value.
 */
void ButtonExtension::event(const char *key, const char *value)
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["event"][key].set(value);
    Device.transmit(doc.as<JsonObjectConst>(), name, false);
}

#if EXTENSION_HOMEASSISTANT
/**
 * @brief Adds Home Assistant device automation triggers for power and mode button presses.
 *
 * @param discovery Home Assistant discovery document to update.
 * @param topic MQTT topic associated with the device.
 * @param unique Device identifier used to construct the discovery topic.
 */
void ButtonExtension::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique)
{
    topic.append(name);
    for (const std::string_view payload : {
             "long",
             "short",
         })
    {
#ifdef PIN_SW1
        {
            const std::string id{std::string(name).append("_power_").append(payload)};
            JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
            component[HomeAssistantAbbreviations::automation_type].set("trigger");
            component[HomeAssistantAbbreviations::payload].set(payload);
            component[HomeAssistantAbbreviations::platform].set("device_automation");
            component[HomeAssistantAbbreviations::subtype].set("Power button");
            component[HomeAssistantAbbreviations::topic].set(topic);
            component[HomeAssistantAbbreviations::type].set(std::string("button_").append(payload).append("_press"));
            component[HomeAssistantAbbreviations::value_template].set("{{value_json.event.power}}");
        }
#endif // PIN_SW1
#ifdef PIN_SW2
        {
            const std::string id{std::string(name).append("_mode_").append(payload)};
            JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
            component[HomeAssistantAbbreviations::automation_type].set("trigger");
            component[HomeAssistantAbbreviations::payload].set(payload);
            component[HomeAssistantAbbreviations::platform].set("device_automation");
            component[HomeAssistantAbbreviations::subtype].set("Mode button");
            component[HomeAssistantAbbreviations::topic].set(topic);
            component[HomeAssistantAbbreviations::type].set(std::string("button_").append(payload).append("_press"));
            component[HomeAssistantAbbreviations::value_template].set("{{value_json.event.mode}}");
        }
#endif // PIN_SW2
    }
}
#endif // EXTENSION_HOMEASSISTANT

#endif // EXTENSION_BUTTON
