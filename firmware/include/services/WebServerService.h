#pragma once

#include <ESPAsyncWebServer.h>

#include "config/constants.h"
#include "modules/ServiceModule.h"
#include "services/NetworkService.h"

class WebServerService : public ServiceModule
{
private:
    static void onOptionsCanonical(AsyncWebServerRequest *request);
    static void onNotFound(AsyncWebServerRequest *request);

public:
    WebServerService() : ServiceModule("WebServer") {};

    AsyncWebServer *http = new AsyncWebServer(80);

    void setup();

    static WebServerService &getInstance();
};

extern WebServerService &WebServer;
