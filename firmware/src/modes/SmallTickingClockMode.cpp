#if MODE_SMALLTICKINGCLOCK

#include <Preferences.h>

#include "config/constants.h"
#include "fonts/MiniFont.h"
#include "handlers/TextHandler.h"
#include "modes/SmallTickingClockMode.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

void SmallTickingClockMode::configure()
{
    Preferences Storage;

    Storage.begin(name, true);

    if (Storage.isKey("ticking"))
    {
        ticking = Storage.getBool("ticking");
        Storage.end();
        transmit();
    }
    else
    {
        Storage.end();
    }
}

void SmallTickingClockMode::begin()
{
    pending = true;
}

void SmallTickingClockMode::handle()
{
    if (getLocalTime(&local))
    {
        if (minute != local.tm_min || hour != local.tm_hour || pending)
        {
            hour = local.tm_hour;
            minute = local.tm_min;
            Display.clearFrame();
            TextHandler(std::to_string(hour / 10), FontMini).draw(GRID_COLUMNS / 2 - 4, GRID_ROWS / 2 - 6);
            TextHandler(std::to_string(hour % 10), FontMini).draw(GRID_COLUMNS / 2 + 1, GRID_ROWS / 2 - 6);
            TextHandler(std::to_string(minute / 10), FontMini).draw(GRID_COLUMNS / 2 - 4, GRID_ROWS / 2);
            TextHandler(std::to_string(minute % 10), FontMini).draw(GRID_COLUMNS / 2 + 1, GRID_ROWS / 2);

            transmit();
            pending = false;
        }

        if (second != local.tm_sec && ticking)
        {
            second = local.tm_sec;
            if (second < 8)
            {
                Display.setPixel(GRID_COLUMNS / 2 - 1 + second, GRID_ROWS / 2 - 8, 0);
                Display.setPixel(GRID_COLUMNS / 2 + second, GRID_ROWS / 2 - 8, INT8_MAX);
            }
            else if (second < 8 + 15)
            {
                Display.setPixel(GRID_COLUMNS / 2 + 7, GRID_ROWS / 2 - 16 + second, 0);
                Display.setPixel(GRID_COLUMNS / 2 + 7, GRID_ROWS / 2 - 15 + second, INT8_MAX);
            }
            else if (second < 8 + 15 * 2)
            {
                Display.setPixel(GRID_COLUMNS / 2 + 30 - second, GRID_ROWS / 2 + 7, 0);
                Display.setPixel(GRID_COLUMNS / 2 + 29 - second, GRID_ROWS / 2 + 7, INT8_MAX);
            }
            else if (second < 8 + 15 * 3)
            {
                Display.setPixel(GRID_COLUMNS / 2 - 8, GRID_ROWS / 2 + 45 - second, 0);
                Display.setPixel(GRID_COLUMNS / 2 - 8, GRID_ROWS / 2 + 44 - second, INT8_MAX);
            }
            else
            {
                Display.setPixel(GRID_COLUMNS / 2 - 61 + second, GRID_ROWS / 2 - 8, 0);
                Display.setPixel(GRID_COLUMNS / 2 - 60 + second, GRID_ROWS / 2 - 8, INT8_MAX);
            }
        }
    }
}

void SmallTickingClockMode::setTicking(const bool _ticking)
{
    ticking = _ticking;
    Preferences Storage;
    Storage.begin(name);
    Storage.putBool("ticking", ticking);
    Storage.end();
    pending = true;
}

void SmallTickingClockMode::transmit()
{
    JsonDocument doc;
    doc["ticking"] = ticking;
    Device.transmit(doc, name);
}

void SmallTickingClockMode::onReceive(const JsonDocument doc, const char *const source)
{
    // Toggle ticking
    if (doc["ticking"].is<bool>())
    {
        setTicking(doc["ticking"].as<bool>());
    }
}

#endif // MODE_SMALLTICKINGCLOCK
