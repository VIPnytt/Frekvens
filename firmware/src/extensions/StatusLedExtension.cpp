#if EXTENSION_STATUSLED

#include "extensions/StatusLedExtension.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)

#include <WiFi.h>

void StatusLedExtension::configure() { pinMode(PIN_LED, OUTPUT); }

void StatusLedExtension::handle()
{
    const unsigned long delta = millis() - lastMillis;
    if ((delta > (1U << 10U) && !digitalRead(PIN_LED)) ||
        (delta > (1U << 9U) && (millis() - lastMillisWarning < (1U << 10U) || !WiFi.isConnected())) ||
        (delta > INT8_MAX && critical))
    {
        lastMillis = millis();
        digitalWrite(PIN_LED, !digitalRead(PIN_LED));
    }
}

void StatusLedExtension::error() { critical = true; }

void StatusLedExtension::warning() { lastMillisWarning = millis(); }

#endif // EXTENSION_STATUSLED
