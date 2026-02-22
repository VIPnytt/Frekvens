#pragma once

#include "modules/ServiceModule.h"

#include <ESPAsyncWebServer.h>

class WebServerService final : public ServiceModule
{
private:
    explicit WebServerService() : ServiceModule("WebServer") {};

    static void onNotFound(AsyncWebServerRequest *request);

public:
    WebServerService(const WebServerService &) = delete;
    WebServerService &operator=(const WebServerService &) = delete;
    WebServerService(WebServerService &&) = delete;
    WebServerService &operator=(WebServerService &&) = delete;
    ~WebServerService() = default;

    AsyncWebServer *http = new AsyncWebServer(80);

    void configure() const;
    void begin() const;

    static void onEmpty(AsyncWebServerRequest *request);

    static WebServerService &getInstance();
};

extern WebServerService &WebServer;
