#include <Preferences.h>

#include "config/constants.h"
#include "extensions/MicrophoneExtension.h"
#include "handlers/BitmapHandler.h"
#include "modes/AnimationMode.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"

void AnimationMode::setup()
{
    Preferences Storage;
    Storage.begin(name, true);
    if (Storage.isKey("duration"))
    {
        duration = Storage.getUShort("duration");
    }
    if (Storage.isKey("frame0"))
    {
        Storage.end();
        transmit();
    }
    else
    {
        Storage.end();
    }
}

void AnimationMode::handle()
{
#if EXTENSION_MICROPHONE
    if (millis() - lastMillis > duration && Microphone->play())
#else
    if (millis() - lastMillis > duration)
#endif
    {
        lastMillis = millis();
        ++index;
        Preferences Storage;
        Storage.begin(name, true);
        if (!Storage.isKey(std::string("frame" + std::to_string(index)).c_str()))
        {
            index = 0;
        }
        if (Storage.isKey(std::string("frame" + std::to_string(index)).c_str()))
        {
            uint8_t frame[COLUMNS * ROWS];
            Storage.getBytes(std::string("frame" + std::to_string(index)).c_str(), frame, COLUMNS * ROWS);
            Display.setFrame(frame);
        }
        Storage.end();
    }
}

void AnimationMode::transmit()
{
    JsonDocument doc;

    Preferences Storage;
    Storage.begin(name, true);
    JsonArray frames = doc["frames"].to<JsonArray>();
    uint8_t i = 0;
    while (Storage.isKey(std::string("frame" + std::to_string(i)).c_str()))
    {
        uint8_t frame[COLUMNS * ROWS];
        Storage.getBytes(std::string("frame" + std::to_string(i)).c_str(), frame, COLUMNS * ROWS);
        for (const uint8_t pixel : frame)
        {
            frames[i].add(pixel);
        }
        if (i == UINT8_MAX)
        {
            break;
        }
        ++i;
    }
    Storage.end();

    doc["duration"] = duration;

    Device.transmit(doc, name);
}

void AnimationMode::receiverHook(const JsonDocument doc)
{
    if (doc["frame"].is<JsonArrayConst>() && doc["frame"].size() == COLUMNS * ROWS && doc["index"].is<uint8_t>())
    {
        uint8_t frame[COLUMNS * ROWS];
        uint8_t i = 0;
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
        Storage.putBytes(std::string("frame" + doc["index"].as<std::string>()).c_str(), frame, COLUMNS * ROWS);
        Storage.end();
        transmit();
#ifdef F_VERBOSE
        Serial.print(name);
        Serial.println(": frame saved");
#endif
    }
    if (doc["duration"].is<uint16_t>())
    {
        duration = doc["duration"].as<uint16_t>();
        Preferences Storage;
        Storage.begin(name);
        Storage.putUShort("duration", duration);
        Storage.end();
    }
    if (doc["length"].is<uint8_t>())
    {
        uint8_t i = doc["length"].as<uint8_t>();
        Preferences Storage;
        Storage.begin(name);
        while (Storage.isKey(std::string("frame" + std::to_string(i)).c_str()))
        {
            Storage.remove(std::string("frame" + std::to_string(i)).c_str());
            if (i == UINT8_MAX)
            {
                break;
            }
            ++i;
        }
        Storage.end();
    }
}
