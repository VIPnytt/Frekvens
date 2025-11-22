#include <HTTPClient.h>
#include <WiFi.h>

#include "services/ConnectivityService.h"
#include "services/WebServerService.h"

void WebServerService::setup()
{
#if EXTENSION_WEBAPP
    http->onNotFound(&onNotFound);
#endif // EXTENSION_WEBAPP
    http->begin();
}

#if EXTENSION_WEBAPP
void WebServerService::onNotFound(AsyncWebServerRequest *request)
{
    if (WiFi.getMode() == wifi_mode_t::WIFI_MODE_AP && request->host() != WiFi.softAPIP().toString())
    {
        ESP_LOGV(WebServer.name, "redirecting");
        request->redirect("http://" + WiFi.softAPIP().toString(), t_http_codes::HTTP_CODE_FOUND);
    }
    else
    {
        ESP_LOGW(WebServer.name, "HTTP 404, %s %s", WebServer.name, request->methodToString(), request->url());
        request->send(t_http_codes::HTTP_CODE_NOT_FOUND);
    }
}
#endif // EXTENSION_WEBAPP

WebServerService &WebServerService::getInstance()
{
    static WebServerService instance;
    return instance;
}

WebServerService &WebServer = WebServerService::getInstance();
