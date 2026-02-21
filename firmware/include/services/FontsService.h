#pragma once

#include "fonts/BrailleFont.h"
#include "fonts/LargeFont.h"
#include "fonts/MediumBoldFont.h"
#include "fonts/MediumFont.h"
#include "fonts/MediumRegularFont.h"
#include "fonts/MicroFont.h"
#include "fonts/MiniFont.h"
#include "fonts/SmallFont.h"
#include "modules/ServiceModule.h"

class FontsService final : public ServiceModule
{
private:
    explicit FontsService() : ServiceModule("Fonts") {};

    const std::vector<FontModule *> modules = {
#if FONT_BRAILLE
        new BrailleFont(),
#endif
        new MicroFont(),
        new MiniFont(),
        new SmallFont(),
        new MediumFont(),
        new MediumRegularFont(),
        new MediumBoldFont(),
        new LargeFont(),
    };

    void transmit();

public:
    void begin();
    [[nodiscard]] const std::vector<FontModule *> &getAll() const;

    static FontsService &getInstance();
};

extern FontsService &Fonts;
