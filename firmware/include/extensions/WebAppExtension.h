#pragma once

#if EXTENSION_WEBAPP

#include "modules/ExtensionModule.h"
#include "services/WebServerService.h"

class WebAppExtension : public ExtensionModule
{
private:
    static void onHeadRoot(AsyncWebServerRequest *request);

public:
    WebAppExtension();

    void configure() override;
    void begin() override;
};

extern WebAppExtension *WebApp;

#endif // EXTENSION_WEBAPP
