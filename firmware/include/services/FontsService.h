#pragma once

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

    static constexpr std::array<std::string_view, 7U> names{
        MicroFont::name,
        MiniFont::name,
        SmallFont::name,
        MediumFont::name,
        MediumBoldFont::name,
        MediumWideFont::name,
        LargeFont::name,
    };

    static constexpr size_t namesMaxLength{[]
                                           {
                                               size_t length{0U};
                                               for (const std::string_view _name : names)
                                               {
                                                   if (_name.size() > length)
                                                   {
                                                       length = _name.size();
                                                   }
                                               }
                                               return length;
                                           }()};

    static FontsService &getInstance();
};

extern FontsService &Fonts; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
