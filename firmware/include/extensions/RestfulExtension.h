#pragma once

#if EXTENSION_RESTFUL

#include <ESPAsyncWebServer.h>

#include "modules/ExtensionModule.h"

class RestfulExtension : public ExtensionModule
{
private:
    static constexpr size_t prefixLength = sizeof("/restful/") - 1;

    static void onGet(AsyncWebServerRequest *request);
    static void onPatch(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);

public:
    RestfulExtension();

    void begin() override;
};

extern RestfulExtension *Restful;

#endif // EXTENSION_RESTFUL
