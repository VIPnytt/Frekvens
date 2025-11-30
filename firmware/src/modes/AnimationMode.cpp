#include "config/constants.h"

#if MODE_ANIMATION

#include <Preferences.h>

#include "extensions/MicrophoneExtension.h"
#include "handlers/BitmapHandler.h"
#include "modes/AnimationMode.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"

void AnimationMode::begin()
{
    index = 0;
    pending = true;
}

void AnimationMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis >= interval && Microphone->isTriggered())
#else
    if (millis() - lastMillis >= interval)
#endif // EXTENSION_MICROPHONE
    {
        Preferences Storage;
        if (!Storage.begin(name, true))
        {
            lastMillis = millis() + UINT16_MAX;
            return;
        }
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
    doc["interval"] = interval;
    JsonArray _frame = doc["frame"].to<JsonArray>();
    for (uint16_t i = 0; i < GRID_COLUMNS * GRID_ROWS; ++i)
    {
        _frame.add(frame[i]);
    }
    doc["index"] = index;
    Device.transmit(doc, name, false);
}

void AnimationMode::onReceive(const JsonDocument doc, const char *const source)
{
    // Action: pull
    if (doc["action"].is<const char *>() && !strcmp(doc["action"].as<const char *>(), "pull"))
    {
        index = 0;
        pending = true;
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
        lastMillis = millis() + GRID_COLUMNS * GRID_ROWS + UINT8_MAX;
        Preferences Storage;
        Storage.begin(name);
        Storage.putBytes(doc["index"].as<std::string>().c_str(), frame, sizeof(frame));
        Storage.end();
        index = 0;
        pending = true;
        ESP_LOGV(name, "frame #%d saved", doc["index"].as<uint8_t>() + 1);
    }
    // Frames
    if (doc["frames"].is<uint8_t>())
    {
        Preferences Storage;
        Storage.begin(name);
        uint8_t i = doc["frames"].as<uint8_t>();
        while (i > 1 && Storage.isKey(std::to_string(i).c_str()))
        {
            Storage.remove(std::to_string(i).c_str());
            ++i;
        }
        Storage.end();
    }
    // Interval
    if (doc["interval"].is<uint16_t>() && interval != doc["interval"].as<uint16_t>())
    {
        interval = doc["interval"].as<uint16_t>();
        Preferences Storage;
        Storage.begin(name);
        Storage.putUShort("interval", interval);
        Storage.end();
    }
}

#endif // MODE_ANIMATION
