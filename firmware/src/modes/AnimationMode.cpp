#if MODE_ANIMATION

#include "modes/AnimationMode.h"

#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)

#include <array>
#include <nvs.h>

void AnimationMode::begin()
{
    index = 0;
    pending = true;
}

void AnimationMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis >= interval && Extensions.Microphone().isTriggered())
#else
    if (millis() - lastMillis >= interval)
#endif // EXTENSION_MICROPHONE
    {
        nvs_handle_t handle{};
        if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
        {
            std::array<uint8_t, GRID_COLUMNS * GRID_ROWS> frame{};
            size_t len{frame.size()}; // NOLINT(cppcoreguidelines-init-variables)
            if (nvs_get_blob(handle, std::to_string(index).c_str(), frame.data(), &len) == ESP_OK)
            {
                nvs_close(handle);
                Display.setFrame(frame);
                if (pending)
                {
                    transmit(index, frame);
                }
                ++index;
            }
            else
            {
                nvs_close(handle);
                index = 0;
                pending = false;
            }
        }
    }
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void AnimationMode::setFrame(uint8_t _index, std::span<const uint8_t> frame)
{
    lastMillis = millis() + (frame.size() * 2);
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_blob(handle, std::to_string(_index).c_str(), frame.data(), frame.size());
        nvs_commit(handle);
        nvs_close(handle);
    }
    index = 0;
    pending = true;
    ESP_LOGV("Status", "frame #%d saved", _index + 1); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
}

void AnimationMode::setFrames(uint8_t count)
{
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        for (uint8_t i = count; i >= 2U; ++i)
        {
            if (nvs_erase_key(handle, std::to_string(i).c_str()) != ESP_OK)
            {
                break;
            }
        }
        nvs_commit(handle);
        nvs_close(handle);
    }
}

void AnimationMode::setInterval(uint16_t _interval)
{
    interval = _interval;
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_u16(handle, "interval", interval);
        nvs_commit(handle);
        nvs_close(handle);
    }
}

void AnimationMode::transmit(uint8_t index, std::span<const uint8_t> frame)
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["interval"].set(interval);
    JsonArray _frame{doc["frame"].to<JsonArray>()};
    for (uint16_t i = 0; i < frame.size(); ++i)
    {
        _frame.add(frame[i]);
    }
    doc["index"].set(index);
    Device.transmit(doc.as<JsonObjectConst>(), name, false);
}

void AnimationMode::onReceive(JsonObjectConst payload,
                              std::string_view source) // NOLINT(misc-unused-parameters)
{
    // Action: pull
    if (payload["action"].is<const char *>() && !strcmp(payload["action"].as<const char *>(), "pull"))
    {
        lastMillis = millis() + (GRID_COLUMNS * GRID_ROWS);
        index = 0;
        pending = true;
    }
    // Frame
    if (payload["frame"].is<JsonArrayConst>() && payload["frame"].size() == GRID_COLUMNS * GRID_ROWS &&
        payload["index"].is<uint8_t>())
    {
        std::array<uint8_t, GRID_COLUMNS * GRID_ROWS> frame{};
        const JsonArrayConst &_frame = payload["frame"].as<JsonArrayConst>();
        for (size_t i = 0; i < frame.size(); ++i)
        {
            if (_frame[i].is<uint8_t>())
            {
                frame[i] = _frame[i].as<uint8_t>();
            }
        }
        setFrame(payload["index"].as<uint8_t>(), frame);
    }
    // Frames
    if (payload["frames"].is<uint8_t>())
    {
        setFrames(payload["frames"].as<uint8_t>());
    }
    // Interval
    if (payload["interval"].is<uint16_t>() && interval != payload["interval"].as<uint16_t>())
    {
        setInterval(payload["interval"].as<uint16_t>());
    }
}

#endif // MODE_ANIMATION
