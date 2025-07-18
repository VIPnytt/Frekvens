#pragma once

#include "config/constants.h"

#if EXTENSION_BUILD

#include "modules/ExtensionModule.h"

class BuildExtension : public ExtensionModule
{
public:
    BuildExtension();

    JsonDocument *config = new JsonDocument();

    void ready() override;
};

extern BuildExtension *Build;

namespace Config
{
    static constexpr std::string_view
        env = ".env",
        h = "secrets.h",
        pio = "platformio.ini";
}

#endif // EXTENSION_BUILD
