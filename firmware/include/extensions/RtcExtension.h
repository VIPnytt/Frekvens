#pragma once

#if EXTENSION_RTC

#include "config/constants.h"
#include "modules/ExtensionModule.h"

#include <RtcDS1307.h>
#include <RtcDS3231.h>
#include <RtcDS3232.h>
#include <RtcPCF8563.h>
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
    RtcDS1307<TwoWire> rtc = RtcDS1307(Wire);
#elif defined(RTC_DS3231)
    RtcDS3231<TwoWire> rtc = RtcDS3231(Wire);
#elif defined(RTC_DS3232)
    RtcDS3232<TwoWire> rtc = RtcDS3232(Wire);
#elif defined(RTC_PCF8563)
    RtcPCF8563<TwoWire> rtc = RtcPCF8563(Wire);
#endif

    void configure() override;
#if defined(RTC_DS3231) || defined(RTC_DS3232) || defined(RTC_PCF8563)
    void handle() override;
#endif
};

extern RtcExtension *Rtc;

#endif // EXTENSION_RTC
