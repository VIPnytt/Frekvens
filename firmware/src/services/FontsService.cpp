#include "services/FontsService.h"

#include "services/DeviceService.h"

void FontsService::begin() { transmit(); }

std::unique_ptr<const FontModule> FontsService::get(std::string_view fontName) const
{
#if FONT_BRAILLE
    if (fontName == BrailleFont::name)
    {
        return std::make_unique<const BrailleFont>();
    }
#endif // FONT_BRAILLE
#if FONT_MICRO
    if (fontName == MicroFont::name)
    {
        return std::make_unique<const MicroFont>();
    }
#endif // FONT_MICRO
#if FONT_MINI
    if (fontName == MiniFont::name)
    {
        return std::make_unique<const MiniFont>();
    }
#endif // FONT_MINI
#if FONT_SMALL
    if (fontName == SmallFont::name)
    {
        return std::make_unique<const SmallFont>();
    }
#endif // FONT_SMALL
#if FONT_MEDIUM
    if (fontName == MediumFont::name)
    {
        return std::make_unique<const MediumFont>();
    }
#endif // FONT_MEDIUM
#if FONT_MEDIUMBOLD
    if (fontName == MediumBoldFont::name)
    {
        return std::make_unique<const MediumBoldFont>();
    }
#endif // FONT_MEDIUMBOLD
#if FONT_MEDIUMWIDE
    if (fontName == MediumWideFont::name)
    {
        return std::make_unique<const MediumWideFont>();
    }
#endif // FONT_MEDIUMWIDE
#if FONT_LARGE
    if (fontName == LargeFont::name)
    {
        return std::make_unique<const LargeFont>();
    }
#endif // FONT_LARGE
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
