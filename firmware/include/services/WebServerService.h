#pragma once

#include "modules/ServiceModule.h"

#include <ESPAsyncWebServer.h>

class WebServerService final : public ServiceModule
{
private:
    explicit WebServerService() : ServiceModule("WebServer") {};

    static void onNotFound(AsyncWebServerRequest *request);

public:
    AsyncWebServer http{80U};

    void configure();
    void begin();

    static void onEmpty(AsyncWebServerRequest *request);

    static WebServerService &getInstance();
};

extern WebServerService &WebServer; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
