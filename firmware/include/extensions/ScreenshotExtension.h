#pragma once

#if EXTENSION_SCREENSHOT

#include "modules/ExtensionModule.h"

class ScreenshotExtension final : public ExtensionModule
{
private:
    static constexpr std::string_view name{"Screenshot"};

    bool pending = false;

    void transmit();

public:
    explicit ScreenshotExtension() : ExtensionModule(name) {};

    void handle() override;

    void onReceive(JsonObjectConst payload, std::string_view source) override;
};

#endif // EXTENSION_SCREENSHOT
