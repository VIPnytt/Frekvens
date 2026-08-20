#if MODE_ANIMATION

#include "modes/AnimationMode.h"

#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)

#include <array>
#include <nvs.h>

void AnimationMode::begin()
{
    index = 0U;
    pending = true;
}

/**
 * @brief Advances the animation and displays the next stored frame when ready.
 *
 * A configured microphone trigger is also required when microphone support is enabled.
 * Missing frames reset playback after the animation has started.
 */
void AnimationMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis >= interval && Extensions.Microphone().isTriggered())
#else
    if (millis() - lastMillis >= interval)
#endif // EXTENSION_MICROPHONE
    {
        nvs_handle_t handle{};
        if (nvs_open(name.data(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
        {
            std::array<uint8_t, GRID_COLUMNS * GRID_ROWS> frame{};
            size_t length{frame.size()};
            if (nvs_get_blob(handle, std::to_string(index).c_str(), frame.data(), &length) == ESP_OK)
            {
                lastMillis = millis();
                Display.setFrame(frame);
                if (pending)
                {
                    transmit(index, frame);
                }
                ++index;
            }
            else if (index == 0U)
            {
                lastMillis = millis() + UINT16_MAX;
            }
            else
            {
                index = 0U;
                pending = false;
            }
            nvs_close(handle);
        }
    }
}

/**
 * @brief Stores an animation frame and restarts playback from the first frame.
 *
 * @param _index Zero-based frame index.
 * @param frame Frame data to store.
 */
void AnimationMode::setFrame(uint8_t _index, std::span<const uint8_t> frame)
{
    lastMillis = millis() + (frame.size() * 2U);
    nvs_handle_t handle{};
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_blob(handle, std::to_string(_index).c_str(), frame.data(), frame.size());
        nvs_commit(handle);
        nvs_close(handle);
    }
    index = 0U;
    pending = true;
    ESP_LOGV(name.data(), "frame #%u saved", _index + 1U); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
}

void AnimationMode::setFrames(uint8_t count)
{
    nvs_handle_t handle{};
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        for (uint8_t idx{count}; idx >= 2U; ++idx)
        {
            if (nvs_find_key(handle, std::to_string(idx).c_str(), nullptr) != ESP_OK ||
                nvs_erase_key(handle, std::to_string(idx).c_str()) != ESP_OK)
            {
                break;
            }
        }
        nvs_commit(handle);
        nvs_close(handle);
    }
}

/**
 * @brief Updates and persists the animation playback interval.
 *
 * @param _interval Playback interval to set.
 */
void AnimationMode::setInterval(uint16_t _interval)
{
    interval = _interval;
    nvs_handle_t handle{};
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_u16(handle, "interval", interval);
        nvs_commit(handle);
        nvs_close(handle);
    }
}

/**
 * @brief Transmits an animation frame and its playback metadata.
 *
 * @param index Frame index.
 * @param frame Frame bytes to transmit.
 */
void AnimationMode::transmit(uint8_t index, std::span<const uint8_t> frame)
{
    JsonDocument doc{};
    for (size_t idx{0U}; idx < frame.size(); ++idx)
    {
        doc["frame"][idx].set(frame[idx]);
    }
    doc["index"].set(index);
    doc["interval"].set(interval);
    Device.transmit(doc.as<JsonObjectConst>(), name, false);
}

/**
 * @brief Applies animation control, frame, frame-count, and interval updates from a payload.
 *
 * @param payload JSON payload containing the requested animation update.
 */
void AnimationMode::onReceive(JsonObjectConst payload, std::string_view source)
{
    // Action: Pull
    if (payload["action"].is<std::string_view>() && payload["action"].as<std::string_view>() == "pull")
    {
        lastMillis = millis() + (GRID_COLUMNS * GRID_ROWS);
        index = 0U;
        pending = true;
    }
    // Frame
    if (payload["frame"].is<JsonArrayConst>() && payload["frame"].size() == GRID_COLUMNS * GRID_ROWS &&
        payload["index"].is<uint8_t>())
    {
        std::array<uint8_t, GRID_COLUMNS * GRID_ROWS> frame{};
        const JsonArrayConst &_frame{payload["frame"].as<JsonArrayConst>()};
        for (size_t idx{0U}; idx < frame.size(); ++idx)
        {
            if (_frame[idx].is<uint8_t>())
            {
                frame[idx] = _frame[idx].as<uint8_t>();
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
