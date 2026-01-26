#pragma once

#include "modules/ServiceModule.h"

#include <ESPAsyncWebServer.h>

class WebServerService : public ServiceModule
{
private:
    static void onNotFound(AsyncWebServerRequest *request);

public:
    WebServerService() : ServiceModule("WebServer") {};

    AsyncWebServer *http = new AsyncWebServer(80);

    void configure();
    void begin();

    static void onEmpty(AsyncWebServerRequest *request);

    static WebServerService &getInstance();
};

extern WebServerService &WebServer;
