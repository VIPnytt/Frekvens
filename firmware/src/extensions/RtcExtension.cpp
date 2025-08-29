#include "config/constants.h"

#if EXTENSION_RTC

#include <esp_sntp.h>

#include "extensions/BuildExtension.h"
#include "extensions/HomeAssistantExtension.h"
#include "extensions/MqttExtension.h"
#include "extensions/RtcExtension.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"

RtcExtension *Rtc = nullptr;

RtcExtension::RtcExtension() : ExtensionModule("RTC")
{
    Rtc = this;
}

void RtcExtension::setup()
{
#ifdef PIN_CS2
    pinMode(PIN_CS2, OUTPUT);
#endif
#ifdef PIN_INT
    pinMode(PIN_INT, INPUT_PULLUP);
#endif
#ifdef PIN_MISO2
    pinMode(PIN_MISO2, INPUT);
#endif
#ifdef PIN_MOSI2
    pinMode(PIN_MOSI2, OUTPUT);
#endif
#ifdef PIN_SCLK2
    pinMode(PIN_MISO2, OUTPUT);
#endif

#if defined(PIN_SCLK2) && defined(PIN_MISO2) && defined(PIN_MOSI2) && defined(PIN_CS2) && defined(RTC_DS3234)
    SPI.begin(PIN_SCLK2, PIN_MISO2, PIN_MOSI2, PIN_CS2);
#endif
#if defined(PIN_SCL) && defined(PIN_SDA) && (defined(RTC_DS1307) || defined(RTC_DS3231) || defined(RTC_DS3232) || defined(RTC_PCF8563))
    rtc.Begin(PIN_SDA, PIN_SCL);
#else
    rtc.Begin();
#endif // defined(PIN_SCL) && defined(PIN_SDA) && (defined(RTC_DS1307) || defined(RTC_DS3231) || defined(RTC_DS3232) || defined(RTC_PCF8563))

    if (rtc.IsDateTimeValid())
    {
        struct timeval tv;
        tv.tv_sec = rtc.GetDateTime().Unix64Time();
        settimeofday(&tv, nullptr);
#ifdef F_VERBOSE
        Serial.printf("%s: sync\n", Rtc->name);
#endif // F_VERBOSE
    }
#ifdef F_DEBUG
    else
    {
        Serial.printf("%s: out of sync\n", name);
    }
#endif // F_INFO
    sntp_set_time_sync_notification_cb(&sntpSetTimeSyncNotificationCallback);

#ifdef PIN_INT
    attachInterrupt(digitalPinToInterrupt(PIN_INT), onInterrupt, CHANGE);
#endif

#if EXTENSION_BUILD && defined(RTC_DS1302)
    (*Build->config)[Config::h][__STRING(RTC_DS1302)] = nullptr;
#elif EXTENSION_BUILD && defined(RTC_DS1307)
    (*Build->config)[Config::h][__STRING(RTC_DS1307)] = nullptr;
#elif EXTENSION_BUILD && defined(RTC_DS3231)
    (*Build->config)[Config::h][__STRING(RTC_DS3231)] = nullptr;
#elif EXTENSION_BUILD && defined(RTC_DS3232)
    (*Build->config)[Config::h][__STRING(RTC_DS3232)] = nullptr;
#elif EXTENSION_BUILD && defined(RTC_DS3234)
    (*Build->config)[Config::h][__STRING(RTC_DS3234)] = nullptr;
#elif EXTENSION_BUILD && defined(RTC_PCF8563)
    (*Build->config)[Config::h][__STRING(RTC_PCF8563)] = nullptr;
#endif // EXTENSION_BUILD && defined(RTC_DS1302)

#if EXTENSION_HOMEASSISTANT && (defined(RTC_DS3231) || defined(RTC_DS3232) || defined(RTC_DS3234))
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_temperature");
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::device_class] = "temperature";
        component[Abbreviations::enabled_by_default] = false;
        component[Abbreviations::expire_after] = UINT8_MAX;
        component[Abbreviations::force_update] = true;
        component[Abbreviations::name] = std::string(name).append(" temperature");
        component[Abbreviations::object_id] = HOSTNAME "_" + id;
        component[Abbreviations::platform] = "sensor";
        component[Abbreviations::state_class] = "measurement";
        component[Abbreviations::state_topic] = topic;
        component[Abbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[Abbreviations::unit_of_measurement] = "°C";
        component[Abbreviations::value_template] = "{{value_json.temperature}}";
    }
#endif // EXTENSION_HOMEASSISTANT && (defined(RTC_DS3231) || defined(RTC_DS3232) || defined(RTC_DS3234))
}

#if defined(RTC_DS3231) || defined(RTC_DS3232) || defined(RTC_DS3234) || defined(RTC_PCF8563)
void RtcExtension::handle()
{
#ifdef PIN_INT
    if (pending)
    {
        if (digitalRead(PIN_INT) == LOW)
        {
#if defined(RTC_DS3231) || defined(RTC_DS3232) || defined(RTC_DS3234)
            rtc.LatchAlarmsTriggeredFlags();
#elif defined(RTC_PCF8563)
            rtc.LatchAlarmTriggeredFlag();
            rtc.LatchTimerTriggeredFlag();
#endif // defined(RTC_DS3231) || defined(RTC_DS3232) || defined(RTC_DS3234)
#ifdef F_INFO
            if (!Display.getPower())
            {
                Serial.printf("%s: power\n", name);
                Display.setPower(true);
            }
#else
            Display.setPower(true);
#endif // F_INFO
        }
        pending = false;
    }
#endif // PIN_INT

#if defined(RTC_DS3231) || defined(RTC_DS3232) || defined(RTC_DS3234)
    if (millis() - lastMillis > UINT16_MAX)
    {
        transmit();
        lastMillis = millis();
    }
#endif // defined(RTC_DS3231) || defined(RTC_DS3232) || defined(RTC_DS3234)
}
#endif // defined(RTC_DS3231) || defined(RTC_DS3232) || defined(RTC_DS3234) || defined(RTC_PCF8563)

#ifdef PIN_INT
IRAM_ATTR void RtcExtension::onInterrupt()
{
    Rtc->pending = true;
}
#endif // PIN_INT

#if defined(RTC_DS3231) || defined(RTC_DS3232) || defined(RTC_DS3234)
void RtcExtension::transmit()
{
    JsonDocument doc;
    doc["temperature"] = rtc.GetTemperature().AsFloatDegC();
    Device.transmit(doc, name);
}
#endif // defined(RTC_DS3231) || defined(RTC_DS3232) || defined(RTC_DS3234)

void RtcExtension::sntpSetTimeSyncNotificationCallback(struct timeval *tv)
{
    time_t timer = tv->tv_sec;
    tm *local = localtime(&timer);
    RtcDateTime dt = RtcDateTime(local->tm_year + 1900, local->tm_mon + 1, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);
    Rtc->rtc.SetDateTime(dt);

#ifdef F_VERBOSE
    Serial.printf("%s: NTP sync\n", Rtc->name);
#endif // F_VERBOSE
}

#endif // EXTENSION_RTC
