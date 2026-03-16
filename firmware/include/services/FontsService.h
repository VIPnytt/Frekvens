#pragma once

#include "fonts/BrailleFont.h"    // NOLINT(misc-include-cleaner)
#include "fonts/LargeFont.h"      // NOLINT(misc-include-cleaner)
#include "fonts/MediumBoldFont.h" // NOLINT(misc-include-cleaner)
#include "fonts/MediumFont.h"     // NOLINT(misc-include-cleaner)
#include "fonts/MediumWideFont.h" // NOLINT(misc-include-cleaner)
#include "fonts/MicroFont.h"      // NOLINT(misc-include-cleaner)
#include "fonts/MiniFont.h"       // NOLINT(misc-include-cleaner)
#include "fonts/SmallFont.h"      // NOLINT(misc-include-cleaner)
#include "modules/ServiceModule.h"

class FontsService final : public ServiceModule
{
private:
    explicit FontsService() : ServiceModule("Fonts") {};

    inline static const std::vector<FontModule *> modules{
#if FONT_BRAILLE
        new BrailleFont(),
#endif
        new MicroFont(),
        new MiniFont(),
        new SmallFont(),
        new MediumFont(),
        new MediumBoldFont(),
        new MediumWideFont(),
        new LargeFont(),
    };

    void transmit();

public:
    void begin();
    [[nodiscard]] const std::vector<FontModule *> &getAll() const;

    static FontsService &getInstance();
};

extern FontsService &Fonts; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
