#include "services/WebServerService.h"

#include <HTTPClient.h>
#include <WiFi.h>

/**
 * @brief Initializes the HTTP server.
 */
void WebServerService::configure() { http.begin(); }

/**
 * @brief Registers the handler for requests that do not match a configured route.
 */
void WebServerService::begin() { http.onNotFound(&onNotFound); }

/**
 * @brief Sends HTTP 404 Not Found for requests that do not match a registered route.
 *
 * @param request Request that could not be matched to a route.
 */
void WebServerService::onNotFound(AsyncWebServerRequest *request)
{
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
