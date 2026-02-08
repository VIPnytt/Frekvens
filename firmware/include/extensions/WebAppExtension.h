#pragma once

#if EXTENSION_WEBAPP

#include "modules/ExtensionModule.h"
#include "services/WebServerService.h"

class WebAppExtension final : public ExtensionModule
{
private:
    static void onHeadRoot(AsyncWebServerRequest *request);

public:
    explicit WebAppExtension();

    void configure() override;
    void begin() override;
};

extern WebAppExtension *WebApp;

#endif // EXTENSION_WEBAPP
