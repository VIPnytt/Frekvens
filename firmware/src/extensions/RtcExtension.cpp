#include "config/constants.h"

#if EXTENSION_RTC

#include <esp_sntp.h>

#include "extensions/HomeAssistantExtension.h"
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
        ESP_LOGV(name, "sync");
    }
    else
    {
        ESP_LOGD(name, "out of sync");
    }
    sntp_set_time_sync_notification_cb(&sntpSetTimeSyncNotificationCallback);

#ifdef PIN_INT
    attachInterrupt(digitalPinToInterrupt(PIN_INT), onInterrupt, CHANGE);
#endif

#if EXTENSION_HOMEASSISTANT && (defined(RTC_DS3231) || defined(RTC_DS3232) || defined(RTC_DS3234))
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_temperature");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::device_class] = "temperature";
        component[HomeAssistantAbbreviations::enabled_by_default] = false;
        component[HomeAssistantAbbreviations::expire_after] = UINT8_MAX;
        component[HomeAssistantAbbreviations::force_update] = true;
        component[HomeAssistantAbbreviations::name] = std::string(name).append(" temperature");
        component[HomeAssistantAbbreviations::object_id] = HOSTNAME "_" + id;
        component[HomeAssistantAbbreviations::platform] = "sensor";
        component[HomeAssistantAbbreviations::state_class] = "measurement";
        component[HomeAssistantAbbreviations::state_topic] = topic;
        component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[HomeAssistantAbbreviations::unit_of_measurement] = "°C";
        component[HomeAssistantAbbreviations::value_template] = "{{value_json.temperature}}";
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
            if (!Display.getPower())
            {
                Display.setPower(true, name);
            }
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
    ESP_LOGV(Rtc->name, "NTP sync");
}

#endif // EXTENSION_RTC
