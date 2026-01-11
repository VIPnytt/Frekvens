#if MODE_LARGETICKINGCLOCK

#include <Preferences.h>

#include "config/constants.h"
#include "fonts/MediumFont.h"
#include "handlers/TextHandler.h"
#include "modes/LargeTickingClockMode.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"

void LargeTickingClockMode::configure()
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

void LargeTickingClockMode::begin()
{
    pending = true;
}

void LargeTickingClockMode::handle()
{
    if (getLocalTime(&local))
    {
        if (minute != local.tm_min || hour != local.tm_hour || pending)
        {
            hour = local.tm_hour;
            minute = local.tm_min;
            Display.clearFrame();
            {
                TextHandler h1 = TextHandler(std::to_string(hour / 10), FontMedium);
                h1.draw(GRID_COLUMNS / 2 - 1 - (7 - h1.getWidth()) / 2 - h1.getWidth(), GRID_ROWS / 2 - 1 - (7 - h1.getHeight()) / 2 - h1.getHeight());
            }
            {
                TextHandler h2 = TextHandler(std::to_string(hour % 10), FontMedium);
                h2.draw(GRID_COLUMNS / 2 + 1 + (7 - h2.getWidth()) / 2, GRID_ROWS / 2 - 1 + (7 - h2.getHeight()) / 2 - h2.getHeight());
            }
            {
                TextHandler m1 = TextHandler(std::to_string(minute / 10), FontMedium);
                m1.draw(GRID_COLUMNS / 2 - 1 - (7 - m1.getWidth()) / 2 - m1.getWidth(), GRID_ROWS / 2 + 1 - (7 - m1.getHeight()) / 2);
            }
            {
                TextHandler m2 = TextHandler(std::to_string(minute % 10), FontMedium);
                m2.draw(GRID_COLUMNS / 2 + 1 + (7 - m2.getWidth()) / 2, GRID_ROWS / 2 + 1 + (7 - m2.getHeight()) / 2);
            }
            transmit();
            pending = false;
        }

        if (second != local.tm_sec && ticking)
        {
            Display.setPixel(GRID_COLUMNS / 2 - 8 + (second + 2) / 4, second % 2 ? GRID_ROWS / 2 : GRID_ROWS / 2 - 1, 0);
            second = local.tm_sec;
            Display.setPixel(GRID_COLUMNS / 2 - 8 + (second + 2) / 4, second % 2 ? GRID_ROWS / 2 : GRID_ROWS / 2 - 1, INT8_MAX);
        }
    }
}

void LargeTickingClockMode::setTicking(const bool _ticking)
{
    ticking = _ticking;
    Preferences Storage;
    Storage.begin(name);
    Storage.putBool("ticking", ticking);
    Storage.end();
    pending = true;
}

void LargeTickingClockMode::transmit()
{
    JsonDocument doc;
    doc["ticking"] = ticking;
    Device.transmit(doc, name);
}

void LargeTickingClockMode::onReceive(const JsonDocument doc, const char *const source)
{
    // Toggle ticking
    if (doc["ticking"].is<bool>())
    {
        setTicking(doc["ticking"].as<bool>());
    }
}

#endif // MODE_LARGETICKINGCLOCK
