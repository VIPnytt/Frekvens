#pragma once

#if EXTENSION_WEBAPP

#include "modules/ExtensionModule.h"

#include <ESPAsyncWebServer.h>

class WebAppExtension final : public ExtensionModule
{
private:
    static constexpr std::string_view name{"Web app"};

    static void onHeadRoot(AsyncWebServerRequest *request);

public:
    explicit WebAppExtension() : ExtensionModule(name) {};

    void configure() override;
    void begin() override;
};

#endif // EXTENSION_WEBAPP
