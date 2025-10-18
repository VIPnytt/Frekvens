#include "config/constants.h"

#if EXTENSION_SIGNAL

#include <Preferences.h>

#include "extensions/SignalExtension.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ModesService.h"

SignalExtension *Signal = nullptr;

SignalExtension::SignalExtension() : ExtensionModule("Signal")
{
    Signal = this;
}

void SignalExtension::ready()
{
    Preferences Storage;
    Storage.begin(name, true);
    if (Storage.isKey("duration"))
    {
        duration = Storage.getUShort("duration");
    }
    Storage.end();
    transmit();
}

void SignalExtension::handle()
{
    if (Display.getPower() && millis() - lastMillis > 1'000 * duration)
    {
        if (signals.size())
        {
            Modes.set(false, name);
            Display.getFrame(frame);
            active = true;

            Display.clear();
            BitmapHandler(signals.front()).draw();
            signals.erase(signals.begin());
            lastMillis = millis();
            Display.flush();
            JsonDocument doc;
            doc["event"] = "signal";
            Device.transmit(doc, name, false);
        }
        else if (active)
        {
            Display.setFrame(frame);
            Modes.set(true, name);
            active = false;
        }
    }
}

void SignalExtension::setDuration(uint8_t seconds)
{
    if (seconds != duration)
    {
        duration = seconds;
        Preferences Storage;
        Storage.begin(name);
        Storage.putUShort("duration", duration);
        Storage.end();
        transmit();
    }
}

void SignalExtension::transmit()
{
    JsonDocument doc;
    doc["duration"] = duration;
    Device.transmit(doc, name);
}

void SignalExtension::receiverHook(const JsonDocument doc)
{
    // Duration
    if (doc["duration"].is<uint8_t>())
    {
        setDuration(doc["duration"].as<uint8_t>());
    }
    // Bitmap
    if (doc["bitmap"].is<JsonArrayConst>())
    {
        std::vector<uint16_t> sign;
        for (const JsonVariantConst bitset : doc["bitmap"].as<JsonArrayConst>())
        {
            if (bitset.is<uint16_t>())
            {
                sign.push_back(bitset.as<uint16_t>());
            }
            else if (bitset.is<std::string>())
            {
                std::string bits = bitset.as<std::string>();
                bits.erase(std::remove_if(bits.begin(), bits.end(), [](char bit)
                                          { return bit < 0x30 || bit > 0x31; }),
                           bits.end());
                sign.push_back(std::stoi(bits, nullptr, 2));
            }
        }
        signals.push_back(sign);
#ifdef F_DEBUG
        Serial.printf("%s: received\n", name);
#endif
    }
}

#endif // EXTENSION_SIGNAL
