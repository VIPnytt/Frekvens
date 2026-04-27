#pragma once

#if EXTENSION_RTC

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ExtensionModule.h"

#include <RtcDS1307.h> // NOLINT(misc-include-cleaner)
#include <RtcDS3231.h> // NOLINT(misc-include-cleaner)
#include <RtcDS3232.h>
#include <RtcPCF8563.h> // NOLINT(misc-include-cleaner)
#include <Wire.h>

class RtcExtension final : public ExtensionModule
{
private:
    static constexpr std::string_view name{"RTC"};

#ifdef PIN_INT
    inline static bool pending = true;
#endif

#if defined(RTC_DS3231) || defined(RTC_DS3232)
    unsigned long lastMillis = 0;
#endif

#if defined(RTC_DS3231) || defined(RTC_DS3232)
    void transmit();
#endif

    static void sntpSetTimeSyncNotificationCallback(struct timeval *tv);

#ifdef PIN_INT
    static void onInterrupt();
#endif

public:
    explicit RtcExtension() : ExtensionModule(name) {};

#ifdef RTC_DS1307
    inline static RtcDS1307<TwoWire> rtc{Wire};
#elif defined(RTC_DS3231)
    inline static RtcDS3231<TwoWire> rtc{Wire};
#elif defined(RTC_DS3232)
    inline static RtcDS3232<TwoWire> rtc{Wire};
#elif defined(RTC_PCF8563)
    inline static RtcPCF8563<TwoWire> rtc{Wire};
#endif // RTC_DS1307

    void configure() override;
#if defined(RTC_DS3231) || defined(RTC_DS3232) || defined(RTC_PCF8563)
    void handle() override;
#endif

#if EXTENSION_HOMEASSISTANT && (defined(RTC_DS3231) || defined(RTC_DS3232))
    void onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) override;
#endif // EXTENSION_HOMEASSISTANT && (defined(RTC_DS3231) || defined(RTC_DS3232))
};

#endif // EXTENSION_RTC
