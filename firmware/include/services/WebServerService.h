#pragma once

#include <ESPAsyncWebServer.h>

#include "config/constants.h"
#include "modules/ServiceModule.h"

class WebServerService : public ServiceModule
{
private:
    static void onOptionsCanonical(AsyncWebServerRequest *request);

#if EXTENSION_WEBAPP || defined(F_VERBOSE)
    static void onNotFound(AsyncWebServerRequest *request);
#endif // EXTENSION_WEBAPP || defined(F_VERBOSE)

public:
    WebServerService() : ServiceModule("WebServer") {};

    AsyncWebServer *http = new AsyncWebServer(80);

    void setup();

    static WebServerService &getInstance();
};

extern WebServerService &WebServer;
