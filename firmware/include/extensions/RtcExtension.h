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
#ifdef PIN_INT
    bool pending = true;
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
    explicit RtcExtension();

#ifdef RTC_DS1307
    RtcDS1307 rtc(Wire);
#elif defined(RTC_DS3231)
    RtcDS3231 rtc(Wire);
#elif defined(RTC_DS3232)
    RtcDS3232 rtc(Wire);
#elif defined(RTC_PCF8563)
    RtcPCF8563 rtc(Wire);
#endif // RTC_DS1307

    void configure() override;
#if defined(RTC_DS3231) || defined(RTC_DS3232) || defined(RTC_PCF8563)
    void handle() override;
#endif
};

extern RtcExtension *Rtc;

#endif // EXTENSION_RTC
