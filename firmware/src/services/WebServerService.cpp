#include "services/WebServerService.h"

#include <HTTPClient.h>
#include <WiFi.h>

void WebServerService::configure() const { http->begin(); }

void WebServerService::begin() const { http->onNotFound(&onNotFound); }

void WebServerService::onNotFound(AsyncWebServerRequest *request)
{
#if EXTENSION_WEBAPP
    if (WiFiClass::getMode() == wifi_mode_t::WIFI_MODE_AP && request->host() != WiFi.softAPIP().toString())
    {
        ESP_LOGV(WebServer.name, "redirecting"); // NOLINT(cppcoreguidelines-avoid-do-while)
        request->redirect("http://" + WiFi.softAPIP().toString(), t_http_codes::HTTP_CODE_FOUND);
    }
    else
#endif // EXTENSION_WEBAPP
    {
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-do-while)
        ESP_LOGW(WebServer.name, "HTTP 404, %s %s", WebServer.name, request->methodToString(), request->url());
        request->send(t_http_codes::HTTP_CODE_NOT_FOUND);
    }
}

void WebServerService::onEmpty(AsyncWebServerRequest *request) {}

WebServerService &WebServerService::getInstance()
{
    static WebServerService instance;
    return instance;
}

// NOLINTNEXTLINE(bugprone-throwing-static-initialization,cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables)
WebServerService &WebServer = WebServerService::getInstance();
