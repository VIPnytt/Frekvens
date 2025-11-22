#include "config/constants.h"

#if MODE_ANIMATION

#include <Preferences.h>

#include "extensions/MicrophoneExtension.h"
#include "handlers/BitmapHandler.h"
#include "modes/AnimationMode.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"

void AnimationMode::wake()
{
    index = 0;
    pending = true;
}

void AnimationMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis >= duration && Microphone->isPlay())
#else
    if (millis() - lastMillis >= duration)
#endif // EXTENSION_MICROPHONE
    {
        Preferences Storage;
        Storage.begin(name, true);
        if (Storage.isKey(std::to_string(index).c_str()))
        {
            lastMillis = millis();
            uint8_t frame[GRID_COLUMNS * GRID_ROWS];
            Storage.getBytes(std::to_string(index).c_str(), frame, sizeof(frame));
            Storage.end();
            Display.setFrame(frame);
            if (pending)
            {
                transmit(index, frame);
            }
            ++index;
        }
        else if (!index)
        {
            Storage.end();
            lastMillis = millis() + UINT16_MAX;
        }
        else
        {
            Storage.end();
            index = 0;
            pending = false;
        }
    }
}

void AnimationMode::transmit(const uint8_t index, const uint8_t frame[GRID_COLUMNS * GRID_ROWS])
{
    JsonDocument doc;
    doc["duration"] = duration;
    JsonArray _frame = doc["frame"].to<JsonArray>();
    for (uint16_t i = 0; i < GRID_COLUMNS * GRID_ROWS; ++i)
    {
        _frame.add(frame[i]);
    }
    doc["index"] = index;
    Device.transmit(doc, name, false);
}

void AnimationMode::receiverHook(const JsonDocument doc, const char *const source)
{
    // Action: pull
    if (doc["action"].is<const char *>() && !strcmp(doc["action"].as<const char *>(), "pull"))
    {
        index = 0;
        pending = true;
    }
    // Duration
    if (doc["duration"].is<uint16_t>() && duration != doc["duration"].as<uint16_t>())
    {
        duration = doc["duration"].as<uint16_t>();
        Preferences Storage;
        Storage.begin(name);
        Storage.putUShort("duration", duration);
        Storage.end();
    }
    // Frame
    if (doc["frame"].is<JsonArrayConst>() && doc["frame"].size() == GRID_COLUMNS * GRID_ROWS && doc["index"].is<uint8_t>())
    {
        uint8_t frame[GRID_COLUMNS * GRID_ROWS];
#if GRID_COLUMNS * GRID_ROWS > (1 << 8)
        uint16_t i = 0;
#else
        uint8_t i = 0;
#endif // GRID_COLUMNS * GRID_ROWS > (1 << 8)
        for (const JsonVariantConst pixel : doc["frame"].as<JsonArrayConst>())
        {
            if (pixel.is<uint8_t>())
            {
                frame[i] = pixel.as<uint8_t>();
            }
            ++i;
        }
        Preferences Storage;
        Storage.begin(name);
        Storage.putBytes(doc["index"].as<std::string>().c_str(), frame, sizeof(frame));
        Storage.end();
        lastMillis = millis() + duration + GRID_COLUMNS * GRID_ROWS;
        index = 0;
        pending = true;
        ESP_LOGV(source, "frame #%d saved", doc["index"].as<uint8_t>() + 1);
    }
    // Frames
    if (doc["frames"].is<uint8_t>())
    {
        Preferences Storage;
        Storage.begin(name);
        if (Storage.isKey(doc["frames"].as<std::string>().c_str()))
        {
            Storage.remove(doc["frames"].as<std::string>().c_str());
        }
        Storage.end();
    }
}

#endif // MODE_ANIMATION
