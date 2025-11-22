#pragma once

#include <ESPAsyncWebServer.h>

#include "config/constants.h"
#include "modules/ServiceModule.h"

class WebServerService : public ServiceModule
{
private:
#if EXTENSION_WEBAPP
    static void onNotFound(AsyncWebServerRequest *request);
#endif

public:
    WebServerService() : ServiceModule("WebServer") {};

    AsyncWebServer *http = new AsyncWebServer(80);

    void setup();

    static WebServerService &getInstance();
};

extern WebServerService &WebServer;
