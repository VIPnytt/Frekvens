#pragma once

#include "fonts/BrailleFont.h"
#include "fonts/LargeFont.h"
#include "fonts/MediumBoldFont.h"
#include "fonts/MediumFont.h"
#include "fonts/MicroFont.h"
#include "fonts/MiniFont.h"
#include "fonts/SmallFont.h"
#include "modules/ServiceModule.h"

class FontsService : public ServiceModule
{
private:
    FontsService() : ServiceModule("Fonts") {};

    const std::vector<FontModule *> modules = {
#if FONT_BRAILLE
        new BrailleFont(),
#endif
        new MicroFont(),
        new MiniFont(),
#if FONT_SMALL
        new SmallFont(),
#endif
        new MediumFont(),
        new MediumBoldFont(),
        new LargeFont(),
    };

    void transmit();

public:
    void ready();
    const std::vector<FontModule *> &getAll() const;

    static FontsService &getInstance();
};

extern FontsService &Fonts;
