#pragma once

#if EXTENSION_WEBAPP

#include "modules/ExtensionModule.h"

#include <ESPAsyncWebServer.h>

class WebAppExtension final : public ExtensionModule
{
private:
    static void onHeadRoot(AsyncWebServerRequest *request);

public:
    explicit WebAppExtension();

    void configure() override;
    void begin() override;
};

extern WebAppExtension *WebApp; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

#endif // EXTENSION_WEBAPP
