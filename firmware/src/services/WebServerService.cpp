#include <HTTPClient.h>
#include <WiFi.h>

#include "services/NetworkService.h"
#include "services/WebServerService.h"

void WebServerService::setup()
{
    http->on("/canonical", WebRequestMethod::HTTP_OPTIONS, &onOptionsCanonical);
    http->onNotFound(&onNotFound);
    http->begin();
}

void WebServerService::onOptionsCanonical(AsyncWebServerRequest *request)
{
    if (std::string_view(request->host().c_str()) == Network.domain)
    {
        AsyncWebServerResponse *response = request->beginResponse(t_http_codes::HTTP_CODE_NO_CONTENT);
        response->addHeader("Access-Control-Allow-Methods", "OPTIONS");
        response->addHeader("Access-Control-Allow-Origin", "*");
        request->send(response);
    }
    else
    {
        request->redirect(std::string("http://").append(Network.domain).append("/canonical").c_str(), t_http_codes::HTTP_CODE_FOUND);
    }
}

void WebServerService::onNotFound(AsyncWebServerRequest *request)
{
    if (WiFi.getMode() == wifi_mode_t::WIFI_MODE_AP)
    {
#ifdef F_VERBOSE
        Serial.print(WebServer.name);
        Serial.println(": redirecting");
#endif
        request->redirect("http://" + WiFi.softAPIP().toString(), t_http_codes::HTTP_CODE_FOUND);
    }
    else
    {
#ifdef F_VERBOSE
        Serial.print(WebServer.name);
        Serial.print(": HTTP 404, ");
        Serial.print(request->methodToString());
        Serial.print(" ");
        Serial.println(request->url());
#endif
        request->send(t_http_codes::HTTP_CODE_NOT_FOUND);
    }
}

WebServerService &WebServerService::getInstance()
{
    static WebServerService instance;
    return instance;
}

WebServerService &WebServer = WebServerService::getInstance();
