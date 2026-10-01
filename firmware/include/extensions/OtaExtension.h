#pragma once

#if EXTENSION_OTA

#include "modules/ExtensionModule.h"

#include <ArduinoOTA.h>

class OtaExtension final : public ExtensionModule
{
private:
    static constexpr std::string_view name{"OTA"};

    ArduinoOTAClass ArduinoOTA;

    static void onStart();
    static void onEnd();

public:
    explicit OtaExtension() : ExtensionModule(name) {};

    void configure() override;
    void begin() override;
    void handle() override;
};

#endif // EXTENSION_OTA
