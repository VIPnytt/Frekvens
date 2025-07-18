#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/FontsService.h"
#include "services/ModesService.h"

void FontsService::ready()
{
    transmit();
}

const std::vector<FontModule *> &FontsService::getAll() const
{
    return modules;
}

void FontsService::transmit()
{
    JsonDocument doc;
    JsonArray list = doc["list"].to<JsonArray>();
    for (const FontModule *font : modules)
    {
        list.add(font->name);
    }
    Device.transmit(doc, name);
}

FontsService &FontsService::getInstance()
{
    static FontsService instance;
    return instance;
}

FontsService &Fonts = Fonts.getInstance();
