#pragma once

#include "fonts/BrailleFont.h"
#include "fonts/LargeFont.h"
#include "fonts/MediumBoldFont.h"
#include "fonts/MediumFont.h"
#include "fonts/MediumWideFont.h"
#include "fonts/MicroFont.h"
#include "fonts/MiniFont.h"
#include "fonts/SmallFont.h"
#include "modules/FontModule.h"
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

    [[nodiscard]] std::unique_ptr<FontModule> get(std::string_view fontName) const;

    static constexpr std::array<std::string_view, COUNT_FONT> names{
#if FONT_BRAILLE
        BrailleFont::name,
#endif
        MicroFont::name,
        MiniFont::name,
        SmallFont::name,
        MediumFont::name,
        MediumBoldFont::name,
        MediumWideFont::name,
        LargeFont::name,
    };

    static FontsService &getInstance();
};

extern FontsService &Fonts; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
