#if EXTENSION_SIGNAL

#include "extensions/SignalExtension.h"

#include "handlers/BitmapHandler.h" // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ModesService.h"

#include <nvs.h>
#include <span>

void SignalExtension::begin()
{
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        nvs_get_u8(handle, "duration", &duration);
        nvs_close(handle);
    }
    transmit();
}

void SignalExtension::handle()
{
    if (Display.getPower() && millis() - lastMillis > 1'000 * duration)
    {
        if (!signals.empty())
        {
            Modes.setActive(false);
            Display.getFrame(frame);
            active = true;

            Display.clearFrame();
            BitmapHandler(std::span<const uint16_t>{signals.front()}).draw();
            signals.erase(signals.begin());
            lastMillis = millis();
            Display.flush();
            JsonDocument doc; // NOLINT(misc-const-correctness)
            doc["event"].set("signal");
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
    duration = seconds;
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_u8(handle, "duration", duration);
        nvs_commit(handle);
        nvs_close(handle);
    }
    transmit();
}

void SignalExtension::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["duration"].set(duration);
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void SignalExtension::onReceive(JsonObjectConst payload,
                                std::string_view source) // NOLINT(misc-unused-parameters)
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
        ESP_LOGD("Queue", "received"); // NOLINT(cppcoreguidelines-pro-type-vararg)
    }
}

#endif // EXTENSION_SIGNAL
