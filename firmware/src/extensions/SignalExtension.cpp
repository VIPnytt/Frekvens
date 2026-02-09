#if EXTENSION_SIGNAL

#include "extensions/SignalExtension.h"

#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ModesService.h"

#include <Preferences.h>

SignalExtension *Signal = nullptr;

SignalExtension::SignalExtension() : ExtensionModule("Signal") { Signal = this; }

void SignalExtension::begin()
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
            Modes.setActive(false);
            Display.getFrame(frame);
            active = true;

            Display.clearFrame();
            BitmapHandler(signals.front()).draw();
            signals.erase(signals.begin());
            lastMillis = millis();
            Display.flush();
            JsonDocument doc;
            doc["event"] = "signal";
            Device.transmit(doc.as<JsonObjectConst>(), name, false);
        }
        else if (active)
        {
            Display.setFrame(frame);
            Modes.setActive(true);
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
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void SignalExtension::onReceive(JsonObjectConst payload, const char *source)
{
    // Duration
    if (payload["duration"].is<uint8_t>())
    {
        setDuration(payload["duration"].as<uint8_t>());
    }
    // Bitmap
    if (payload["bitmap"].is<JsonArrayConst>())
    {
        std::vector<uint16_t> sign;
        for (const JsonVariantConst bitset : payload["bitmap"].as<JsonArrayConst>())
        {
            if (bitset.is<uint16_t>())
            {
                sign.push_back(bitset.as<uint16_t>());
            }
            else if (bitset.is<std::string>())
            {
                std::string bits = bitset.as<std::string>();
                bits.erase(std::remove_if(bits.begin(), bits.end(), [](char bit) { return bit < 0x30 || bit > 0x31; }),
                           bits.end());
                sign.push_back(std::stoi(bits, nullptr, 2));
            }
        }
        signals.push_back(sign);
        ESP_LOGD(name, "received");
    }
}

#endif // EXTENSION_SIGNAL
