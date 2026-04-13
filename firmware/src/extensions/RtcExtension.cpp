#if EXTENSION_RTC

#include "extensions/RtcExtension.h"

#include "extensions/HomeAssistantExtension.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"

#include <esp_sntp.h>

void RtcExtension::configure()
{
#ifdef PIN_INT
    pinMode(PIN_INT, INPUT_PULLUP);
#endif
#if defined(RTC_DS1307) || defined(RTC_DS3231) || defined(RTC_DS3232) || defined(RTC_PCF8563)
    rtc.Begin(PIN_SDA, PIN_SCL);
#endif
    if (rtc.IsDateTimeValid())
    {
        tm local{};
        if (!getLocalTime(&local))
        {
            struct timeval tv{}; // NOLINT(misc-const-correctness)
            tv.tv_sec = rtc.GetDateTime().Unix64Time();
            settimeofday(&tv, nullptr);
            ESP_LOGD(name, "sync"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        }
    }
    else
    {
        ESP_LOGW(name, "out of sync"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    }
    sntp_set_time_sync_notification_cb(&sntpSetTimeSyncNotificationCallback);

#ifdef PIN_INT
    attachInterrupt(PIN_INT, onInterrupt, CHANGE);
#endif

#if EXTENSION_HOMEASSISTANT && (defined(RTC_DS3231) || defined(RTC_DS3232))
    const std::string topic{std::string("frekvens/" HOSTNAME "/").append(name)};
    const HomeAssistantExtension &_ha = Extensions.HomeAssistant();
    {
        const std::string id{std::string(name).append("_temperature")};
        JsonObject component{(*_ha.discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::device_class].set("temperature");
        component[HomeAssistantAbbreviations::enabled_by_default].set(false);
        component[HomeAssistantAbbreviations::expire_after].set(UINT8_MAX);
        component[HomeAssistantAbbreviations::force_update].set(true);
        component[HomeAssistantAbbreviations::name].set(std::string(name).append(" temperature"));
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        component[HomeAssistantAbbreviations::platform].set("sensor");
        component[HomeAssistantAbbreviations::state_class].set("measurement");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(_ha.uniquePrefix + id);
        component[HomeAssistantAbbreviations::unit_of_measurement].set("°C");
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.temperature}}");
    }
#endif // EXTENSION_HOMEASSISTANT && (defined(RTC_DS3231) || defined(RTC_DS3232))
}

#if defined(RTC_DS3231) || defined(RTC_DS3232) || defined(RTC_PCF8563)
void RtcExtension::handle()
{
#ifdef PIN_INT
    if (pending)
    {
        if (digitalRead(PIN_INT) == LOW)
        {
#if defined(RTC_DS3231) || defined(RTC_DS3232)
            rtc.LatchAlarmsTriggeredFlags();
#elif defined(RTC_PCF8563)
            rtc.LatchAlarmTriggeredFlag();
            rtc.LatchTimerTriggeredFlag();
#endif // defined(RTC_DS3231) || defined(RTC_DS3232)
            Display.setPower(true);
        }
        pending = false;
    }
#endif // PIN_INT
#if defined(RTC_DS3231) || defined(RTC_DS3232)
    if (millis() - lastMillis > UINT16_MAX)
    {
        transmit();
        lastMillis = millis();
    }
#endif // defined(RTC_DS3231) || defined(RTC_DS3232)
}
#endif // defined(RTC_DS3231) || defined(RTC_DS3232) || defined(RTC_PCF8563)

#ifdef PIN_INT
IRAM_ATTR void RtcExtension::onInterrupt() { pending = true; }
#endif // PIN_INT

#if defined(RTC_DS3231) || defined(RTC_DS3232)
void RtcExtension::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["temperature"].set(rtc.GetTemperature().AsFloatDegC());
    Device.transmit(doc.as<JsonObjectConst>(), name);
}
#endif // defined(RTC_DS3231) || defined(RTC_DS3232)

void RtcExtension::sntpSetTimeSyncNotificationCallback(struct timeval *tv)
{
    const time_t timer = tv->tv_sec;
    const tm *local = gmtime(&timer);
    rtc.SetDateTime(RtcDateTime(
        local->tm_year + 1900, local->tm_mon + 1, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec));
    ESP_LOGV(name, "NTP sync"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
}

#endif // EXTENSION_RTC
