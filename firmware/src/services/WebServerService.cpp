#include "services/WebServerService.h"

#include <HTTPClient.h>
#include <WiFi.h>

void WebServerService::configure() const { http->begin(); }

void WebServerService::begin() const
{
#if EXTENSION_WEBAPP
    http->onNotFound(&onNotFound);
#endif // EXTENSION_WEBAPP
}

void WebServerService::onNotFound(AsyncWebServerRequest *request)
{
#if EXTENSION_WEBAPP
    if (WiFiClass::getMode() == wifi_mode_t::WIFI_MODE_AP)
    {
        ESP_LOGV(WebServer.name.data(), "HTTP 302 Found"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        request->redirect("http://192.168.4.1", t_http_codes::HTTP_CODE_FOUND);
    }
    else
#endif // EXTENSION_WEBAPP
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        ESP_LOGD(WebServer.name.data(), "HTTP 404 Not Found, %s %s", request->methodToString(), request->url().c_str());
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
WebServerService &WebServer{WebServerService::getInstance()};
