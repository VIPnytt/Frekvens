#pragma once

#include "config/constants.h"

#if EXTENSION_RESTFUL

#include <ESPAsyncWebServer.h>

#include "modules/ExtensionModule.h"

class RestfulExtension : public ExtensionModule
{
private:
    static constexpr size_t prefixLength = sizeof("/RESTful/");

    static void onGet(AsyncWebServerRequest *request);
    static void onGetModule(AsyncWebServerRequest *request);
    static void onPatchModule(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);

public:
    RestfulExtension();

    void ready() override;
};

extern RestfulExtension *Restful;

#endif // EXTENSION_RESTFUL
