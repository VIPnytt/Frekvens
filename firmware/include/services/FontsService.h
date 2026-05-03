#pragma once

#include "fonts/BrailleFont.h"    //NOLINT(misc-include-cleaner)
#include "fonts/LargeFont.h"      //NOLINT(misc-include-cleaner)
#include "fonts/MediumBoldFont.h" //NOLINT(misc-include-cleaner)
#include "fonts/MediumFont.h"     //NOLINT(misc-include-cleaner)
#include "fonts/MediumWideFont.h" //NOLINT(misc-include-cleaner)
#include "fonts/MicroFont.h"      //NOLINT(misc-include-cleaner)
#include "fonts/MiniFont.h"       //NOLINT(misc-include-cleaner)
#include "fonts/SmallFont.h"      //NOLINT(misc-include-cleaner)
#include "modules/ServiceModule.h"

#include <array>
#include <bits/unique_ptr.h>
#include <string_view>

class FontsService final : public ServiceModule
{
private:
    explicit FontsService() : ServiceModule("Fonts") {};

    void transmit();

public:
    void begin();

    [[nodiscard]] std::unique_ptr<const FontModule> get(std::string_view fontName) const;

    static constexpr auto names = std::to_array<std::string_view>({
#if FONT_BRAILLE
        BrailleFont::name,
#endif // FONT_BRAILLE
#if FONT_MICRO
        MicroFont::name,
#endif // FONT_MICRO
#if FONT_MINI
        MiniFont::name,
#endif // FONT_MINI
#if FONT_SMALL
        SmallFont::name,
#endif // FONT_SMALL
#if FONT_MEDIUM
        MediumFont::name,
#endif // FONT_MEDIUM
#if FONT_MEDIUMBOLD
        MediumBoldFont::name,
#endif // FONT_MEDIUMBOLD
#if FONT_MEDIUMWIDE
        MediumWideFont::name,
#endif // FONT_MEDIUMWIDE
#if FONT_LARGE
        LargeFont::name,
#endif // FONT_LARGE
    });

    static FontsService &getInstance();
};

extern FontsService &Fonts; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
