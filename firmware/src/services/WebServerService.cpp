#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <WiFi.h>

#include "services/ConnectivityService.h"
#include "services/WebServerService.h"

void WebServerService::configure()
{
    http->begin();
}

void WebServerService::begin()
{
    http->onNotFound(&onNotFound);
}

void WebServerService::onNotFound(AsyncWebServerRequest *request)
{
#if EXTENSION_WEBAPP
    if (WiFi.getMode() == wifi_mode_t::WIFI_MODE_AP && request->host() != WiFi.softAPIP().toString())
    {
        ESP_LOGV(WebServer.name, "redirecting");
        request->redirect("http://" + WiFi.softAPIP().toString(), t_http_codes::HTTP_CODE_FOUND);
    }
    else
#endif // EXTENSION_WEBAPP
    {
        ESP_LOGW(WebServer.name, "HTTP 404, %s %s", WebServer.name, request->methodToString(), request->url());
        request->send(t_http_codes::HTTP_CODE_NOT_FOUND);
    }
}

WebServerService &WebServerService::getInstance()
{
    static WebServerService instance;
    return instance;
}

WebServerService &WebServer = WebServerService::getInstance();
