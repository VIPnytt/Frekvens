#pragma once

#include "config/constants.h"

#if EXTENSION_RTC

#include <RtcDS1302.h>
#include <RtcDS1307.h>
#include <RtcDS3231.h>
#include <RtcDS3232.h>
#include <RtcDS3234.h>
#include <RtcPCF8563.h>
#include <SPI.h>
#include <Wire.h>

#include "modules/ExtensionModule.h"

class RtcExtension : public ExtensionModule
{
private:
#ifdef PIN_INT
    bool pending = true;
#endif

#if defined(RTC_DS3231) || defined(RTC_DS3232) || defined(RTC_DS3234)
    unsigned long lastMillis = 0;
#endif

#if defined(RTC_DS3231) || defined(RTC_DS3232) || defined(RTC_DS3234)
    void transmit();
#endif

    static void sntpSetTimeSyncNotificationCallback(struct timeval *tv);

#ifdef PIN_INT
    static void onInterrupt();
#endif

public:
    RtcExtension();

#ifdef RTC_DS1302
    ThreeWire spiDio = ThreeWire(PIN_SDIO2, PIN_SCLK2, PIN_CS2);
    RtcDS1302<ThreeWire> rtc = RtcDS1302(spiDio);
#elif defined(RTC_DS1307)
    RtcDS1307<TwoWire> rtc = RtcDS1307(Wire);
#elif defined(RTC_DS3231)
    RtcDS3231<TwoWire> rtc = RtcDS3231(Wire);
#elif defined(RTC_DS3232)
    RtcDS3232<TwoWire> rtc = RtcDS3232(Wire);
#elif defined(RTC_DS3234)
    RtcDS3234<SPIClass> rtc = RtcDS3234(SPI, PIN_CS2);
#elif defined(RTC_PCF8563)
    RtcPCF8563<TwoWire> rtc = RtcPCF8563(Wire);
#endif

    void setup() override;
#if defined(RTC_DS3231) || defined(RTC_DS3232) || defined(RTC_DS3234) || defined(RTC_PCF8563)
    void handle() override;
#endif
};

extern RtcExtension *Rtc;

#endif // EXTENSION_RTC
