#include "services/FontsService.h"

#include "services/DeviceService.h"

void FontsService::begin() { transmit(); }

std::unique_ptr<FontModule> FontsService::get(std::string_view fontName) const
{
#if FONT_BRAILLE
    if (fontName == BrailleFont::name)
    {
        return std::make_unique<BrailleFont>();
    }
#endif
    if (fontName == MicroFont::name)
    {
        return std::make_unique<MicroFont>();
    }
    if (fontName == MiniFont::name)
    {
        return std::make_unique<MiniFont>();
    }
    if (fontName == SmallFont::name)
    {
        return std::make_unique<SmallFont>();
    }
    if (fontName == MediumFont::name)
    {
        return std::make_unique<MediumFont>();
    }
    if (fontName == MediumBoldFont::name)
    {
        return std::make_unique<MediumBoldFont>();
    }
    if (fontName == MediumWideFont::name)
    {
        return std::make_unique<MediumWideFont>();
    }
    if (fontName == LargeFont::name)
    {
        return std::make_unique<LargeFont>();
    }
    return nullptr;
}

void FontsService::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    JsonArray list{doc["list"].to<JsonArray>()};
    for (const std::string_view _name : names)
    {
        list.add(_name);
    }
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

FontsService &FontsService::getInstance()
{
    static FontsService instance;
    return instance;
}

// NOLINTNEXTLINE(bugprone-throwing-static-initialization,cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables)
FontsService &Fonts = FontsService::getInstance();
