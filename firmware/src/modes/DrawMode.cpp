#if MODE_DRAW

#include "modes/DrawMode.h"

#include "services/DeviceService.h"
#include "services/DisplayService.h"

#include <nvs.h>

void DrawMode::begin()
{
    load(true);
    if (!render)
    {
        for (const uint8_t pixel : frame)
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
        Display.setFrame(frame);
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
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        size_t len{frame.size()}; // NOLINT(cppcoreguidelines-init-variables)
        const char *key = nullptr;
        if ((cache && nvs_get_blob(handle, "cache", frame.data(), &len) == ESP_OK) ||
            nvs_get_blob(handle, "saved", frame.data(), &len) == ESP_OK)
        {
            pending = true;
            render = true;
        }
        nvs_close(handle);
    }
}

void DrawMode::save(bool cache)
{
    if (std::any_of(frame.begin(), frame.end(), [](uint8_t pixel) { return pixel > 0; }))
    {
        nvs_handle_t handle{};
        if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
        {
            nvs_set_blob(handle, cache ? "cache" : "saved", frame.data(), frame.size());
            nvs_commit(handle);
            nvs_close(handle);
        }
        pending = true;
        if (!cache)
        {
            ESP_LOGV("Status", "saved"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        }
    }
}

void DrawMode::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    JsonArray _frame{doc["frame"].to<JsonArray>()};
    for (const uint8_t pixel : frame)
    {
        _frame.add(pixel);
    }
    Device.transmit(doc.as<JsonObjectConst>(), name, false);
}

void DrawMode::onReceive(JsonObjectConst payload,
                         std::string_view source) // NOLINT(misc-unused-parameters)
{
    // Action
    if (payload["action"].is<std::string_view>())
    {
        const std::string_view action{payload["action"].as<std::string_view>()};
        // Clear
        if (action == "clear")
        {
            frame.fill(0);
            render = true;
        }
        // Load
        else if (action == "load")
        {
            load();
        }
        // Pull
        else if (action == "pull")
        {
            save(true);
        }
        // Save
        else if (action == "save")
        {
            save();
        }
    }
    // Frame
    if (payload["frame"].is<JsonArrayConst>() && payload["frame"].size() == frame.size())
    {
        const JsonArrayConst _frame = payload["frame"].as<JsonArrayConst>();
        for (size_t i = 0; i < frame.size(); ++i)
        {
            if (_frame[i].is<uint8_t>())
            {
                frame[i] = _frame[i].as<uint8_t>();
            }
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
                const uint8_t x = pixel["x"].as<uint8_t>();
                const uint8_t y = pixel["y"].as<uint8_t>();
                if (x < GRID_COLUMNS && y < GRID_ROWS)
                {
                    frame[x + y * GRID_COLUMNS] = pixel["brightness"].as<uint8_t>();
                }
            }
        }
        render = true;
    }
}

#endif // MODE_DRAW
