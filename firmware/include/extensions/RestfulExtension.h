#pragma once

#include "config/constants.h"

#if EXTENSION_RESTFUL

#include <ESPAsyncWebServer.h>

#include "modules/ExtensionModule.h"

class RestfulExtension : public ExtensionModule
{
private:
    static constexpr size_t prefixLength = sizeof("/api");

    static void onGetApi(AsyncWebServerRequest *request);
    static void onGetApiModule(AsyncWebServerRequest *request);
    static void onSetApiModule(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);

public:
    RestfulExtension();

    void ready() override;
};

extern RestfulExtension *Restful;

#endif // EXTENSION_RESTFUL
