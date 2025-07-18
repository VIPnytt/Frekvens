#include <Preferences.h>

#include "handlers/BitmapHandler.h"
#include "modes/DrawMode.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"

void DrawMode::setup()
{
    load(true);
}

void DrawMode::wake()
{
    if (!pending)
    {
        for (const uint8_t pixel : drawing)
        {
            if (pixel)
            {
                pending = true;
                break;
            }
        }
    }
}

void DrawMode::handle()
{
    if (pending)
    {
        pending = false;
        Display.setFrame(drawing);
    }
}

void DrawMode::sleep()
{
    save(true);
}

void DrawMode::transmit()
{
    JsonDocument doc;
    JsonArray frame = doc["frame"].to<JsonArray>();
    for (const uint8_t pixel : drawing)
    {
        frame.add(pixel);
    }
    Device.transmit(doc, name);
}

void DrawMode::receiverHook(const JsonDocument doc)
{
    if (doc["action"].is<const char *>())
    {
        const char *const action = doc["action"].as<const char *>();
        // Clear
        if (!strcmp(action, "clear"))
        {
            memset(drawing, 0, sizeof(drawing));
            pending = true;
        }
        // Load
        else if (!strcmp(action, "load"))
        {
            load();
        }
        // Pull
        else if (!strcmp(action, "pull"))
        {
            save(true);
        }
        // Save
        else if (!strcmp(action, "save"))
        {
            save();
        }
    }
    // Frame
    if (doc["frame"].is<JsonArrayConst>() && doc["frame"].size() == COLUMNS * ROWS)
    {
        uint8_t i = 0;
        for (const JsonVariantConst pixel : doc["frame"].as<JsonArrayConst>())
        {
            if (pixel.is<uint8_t>())
            {
                drawing[i] = pixel.as<uint8_t>();
            }
            ++i;
        }
        save(true);
        pending = true;
#ifdef F_DEBUG
        Serial.print(name);
        Serial.println(": frame");
#endif
    }
    // Pixel
    if (doc["pixels"].is<JsonArrayConst>())
    {
        for (const JsonVariantConst &pixel : doc["pixels"].as<JsonArrayConst>())
        {
            if (pixel["x"].is<uint8_t>() && pixel["y"].is<uint8_t>() && pixel["value"].is<uint8_t>())
            {
                drawing[pixel["x"].as<uint8_t>() + pixel["y"].as<uint8_t>() * COLUMNS] = pixel["value"].as<uint8_t>();
            }
        }
        pending = true;
    }
}

void DrawMode::load(bool cache)
{
    Preferences Storage;
    Storage.begin(name, true);
    const char *const key = cache ? "cache" : "saved";
    if (Storage.isKey(key))
    {
        Storage.getBytes(key, drawing, COLUMNS * ROWS);
        Storage.end();
        pending = true;
        transmit();
    }
    else
    {
        Storage.end();
        if (cache)
        {
            load(!cache);
        }
    }
}

void DrawMode::save(bool cache)
{
    for (const uint8_t pixel : drawing)
    {
        if (pixel)
        {
            Preferences Storage;
            Storage.begin(name);
            Storage.putBytes(cache ? "cache" : "saved", drawing, COLUMNS * ROWS);
            Storage.end();
#ifdef F_VERBOSE
            Serial.print(name);
            Serial.println(cache ? ": cached" : ": saved");
#endif
            break;
        }
    }
    transmit();
}
