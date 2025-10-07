#include <HTTPClient.h>
#include <WiFi.h>

#include "services/ConnectivityService.h"
#include "services/WebServerService.h"

void WebServerService::setup()
{
    http->on("/canonical", WebRequestMethod::HTTP_OPTIONS, &onOptionsCanonical);

#if EXTENSION_WEBAPP
    http->onNotFound(&onNotFound);
#endif // EXTENSION_WEBAPP

    http->begin();
}

void WebServerService::onOptionsCanonical(AsyncWebServerRequest *request)
{
    if (std::string_view(request->host().c_str()) == Connectivity.domain)
    {
        AsyncWebServerResponse *response = request->beginResponse(t_http_codes::HTTP_CODE_NO_CONTENT);
        response->addHeader("Access-Control-Allow-Methods", "OPTIONS");
        response->addHeader("Access-Control-Allow-Origin", "*");
        request->send(response);
    }
    else
    {
        request->redirect(std::string("http://").append(Connectivity.domain).append("/canonical").c_str(), t_http_codes::HTTP_CODE_FOUND);
    }
}

#if EXTENSION_WEBAPP || defined(F_VERBOSE)
void WebServerService::onNotFound(AsyncWebServerRequest *request)
{
#if EXTENSION_WEBAPP
    if (WiFi.getMode() == wifi_mode_t::WIFI_MODE_AP && request->host() != WiFi.softAPIP().toString() && request->host() != WiFi.softAPIPv6().toString())
    {
#ifdef F_VERBOSE
        Serial.printf("%s: redirecting\n", WebServer.name);
#endif // F_VERBOSE
        request->redirect("http://" + WiFi.softAPIP().toString(), t_http_codes::HTTP_CODE_FOUND);
    }
    else
#endif // EXTENSION_WEBAPP
    {
#ifdef F_VERBOSE
        Serial.printf("%s: HTTP 404, %s %s\n", WebServer.name, request->methodToString(), request->url());
#endif // F_VERBOSE
        request->send(t_http_codes::HTTP_CODE_NOT_FOUND);
    }
}
#endif // EXTENSION_WEBAPP || defined(F_VERBOSE)

WebServerService &WebServerService::getInstance()
{
    static WebServerService instance;
    return instance;
}

WebServerService &WebServer = WebServerService::getInstance();
