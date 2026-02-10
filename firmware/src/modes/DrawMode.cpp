#if MODE_DRAW

#include "modes/DrawMode.h"

#include "handlers/BitmapHandler.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ModesService.h"

#include <Preferences.h>

void DrawMode::begin()
{
    load(true);
    if (!render)
    {
        for (const uint8_t pixel : drawing)
        {
            if (pixel > 0)
            {
                pending = true;
                render = true;
                break;
            }
        }
    }
}

void DrawMode::handle()
{
    if (render)
    {
        Display.setFrame(drawing);
        render = false;
    }
    else if (pending)
    {
        transmit();
        pending = false;
    }
}

void DrawMode::end() { save(true); }

void DrawMode::load(bool cache)
{
    Preferences Storage;
    Storage.begin(name, true);
    const char *const key = cache ? "cache" : "saved";
    if (cache && Storage.isKey("cache"))
    {
        Storage.getBytes("cache", static_cast<void *>(drawing), sizeof(drawing));
        pending = true;
        render = true;
    }
    else if (Storage.isKey("saved"))
    {
        Storage.getBytes("saved", static_cast<void *>(drawing), sizeof(drawing));
        pending = true;
        render = true;
    }
    Storage.end();
}

void DrawMode::save(bool cache)
{
    for (const uint8_t pixel : drawing)
    {
        if (pixel > 0)
        {
            Preferences Storage;
            Storage.begin(name);
            Storage.putBytes(cache ? "cache" : "saved", static_cast<void *>(drawing), sizeof(drawing));
            Storage.end();
            pending = true;
            if (!cache)
            {
                ESP_LOGV(name, "saved");
            }
            break;
        }
    }
}

void DrawMode::transmit()
{
    JsonDocument doc;
    JsonArray frame = doc["frame"].to<JsonArray>();
    for (const uint8_t pixel : drawing)
    {
        frame.add(pixel);
    }
    Device.transmit(doc.as<JsonObjectConst>(), name, false);
}

void DrawMode::onReceive(JsonObjectConst payload, const char *source)
{
    if (payload["action"].is<const char *>())
    {
        const char *const action = payload["action"].as<const char *>();
        // Clear
        if (strcmp(action, "clear") == 0)
        {
            memset(drawing, 0, sizeof(drawing));
            render = true;
        }
        // Load
        else if (strcmp(action, "load") == 0)
        {
            load();
        }
        // Pull
        else if (strcmp(action, "pull") == 0)
        {
            save(true);
        }
        // Save
        else if (strcmp(action, "save") == 0)
        {
            save();
        }
    }
    // Frame
    if (payload["frame"].is<JsonArrayConst>() && payload["frame"].size() == GRID_COLUMNS * GRID_ROWS)
    {
#if GRID_COLUMNS * GRID_ROWS > (1 << 8)
        uint16_t i = 0;
#else
        uint8_t i = 0;
#endif // GRID_COLUMNS * GRID_ROWS > (1 << 8)
        for (const JsonVariantConst pixel : payload["frame"].as<JsonArrayConst>())
        {
            if (pixel.is<uint8_t>())
            {
                drawing[i] = pixel.as<uint8_t>();
            }
            ++i;
        }
        render = true;
    }
    // Pixel
    if (payload["pixels"].is<JsonArrayConst>())
    {
        for (const JsonVariantConst &pixel : payload["pixels"].as<JsonArrayConst>())
        {
            if (pixel["x"].is<uint8_t>() && pixel["y"].is<uint8_t>() && pixel["brightness"].is<uint8_t>())
            {
                drawing[pixel["x"].as<uint8_t>() + pixel["y"].as<uint8_t>() * GRID_COLUMNS] =
                    pixel["brightness"].as<uint8_t>();
            }
        }
        render = true;
    }
}

#endif // MODE_DRAW
