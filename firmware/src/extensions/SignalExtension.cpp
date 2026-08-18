#if EXTENSION_SIGNAL

#include "extensions/SignalExtension.h"

#include "handlers/BitmapHandler.h" // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ModesService.h"

#include <nvs.h>
#include <span>

/**
 * @brief Loads the stored signal duration and transmits the current configuration.
 */
void SignalExtension::begin()
{
    nvs_handle_t handle{};
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        uint8_t _duration{0U};
        if (nvs_get_u8(handle, "duration", &_duration) == ESP_OK)
        {
            duration = _duration * 1'000U;
        }
        nvs_close(handle);
    }
    transmit();
}

/**
 * @brief Displays the next queued signal bitmap or restores the previous frame.
 *
 * Processes signals only when the display is powered on and the configured
 * display interval has elapsed.
 */
void SignalExtension::handle()
{
    if (Display.getPower() && millis() - lastMillis > duration)
    {
        if (!signals.empty())
        {
            Modes.setActive(false);
            Display.getFrame(frame);
            active = true;
            Display.fillFrame(0U);
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
    const uint32_t _duration{seconds * 1'000U};
    if (_duration != duration && _duration != 0U)
    {
        duration = _duration;
        nvs_handle_t handle{};
        if (nvs_open(name.data(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
        {
            nvs_set_u8(handle, "duration", seconds);
            nvs_commit(handle);
            nvs_close(handle);
        }
        transmit();
    }
}

/**
 * @brief Transmits the configured signal duration in seconds.
 */
void SignalExtension::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["duration"].set(duration / 1'000U);
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

/**
 * @brief Processes incoming duration and bitmap configuration data.
 *
 * Numeric bitmap elements are stored as 16-bit values, while string elements
 * are filtered to binary characters and interpreted as base-2 values.
 *
 * @param payload Incoming configuration data.
 */
void SignalExtension::onReceive(JsonObjectConst payload, std::string_view source)
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
        ESP_LOGD("Queue", "received"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    }
}

#endif // EXTENSION_SIGNAL
