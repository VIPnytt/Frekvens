#include "config/constants.h"

#if EXTENSION_WEBAPP

#include <HTTPClient.h>
#include <LittleFS.h>
#include <SPIFFS.h>

#include "extensions/WebAppExtension.h"
#include "services/DeviceService.h"

WebAppExtension *WebApp = nullptr;

WebAppExtension::WebAppExtension() : ExtensionModule("Web app")
{
    WebApp = this;
}

void WebAppExtension::setup()
{
#ifdef F_VERBOSE
#ifdef BOARD_BUILD__FILESYSTEM__LITTLEFS
    if (LittleFS.begin() && LittleFS.exists("/webapp/v" VERSION ".html.gz"))
    {
        WebServer.http->serveStatic("/", LittleFS, "/webapp/", "max-age=60").setDefaultFile("v" VERSION ".html");
    }
#else
    if (SPIFFS.begin() && SPIFFS.exists("/webapp/v" VERSION ".html.gz"))
    {
        WebServer.http->serveStatic("/", SPIFFS, "/webapp/", "max-age=60").setDefaultFile("v" VERSION ".html");
    }
#endif // BOARD_BUILD__FILESYSTEM__LITTLEFS
    else
    {
        Serial.printf("%s: not found\n", name);
    }
#else
#ifdef BOARD_BUILD__FILESYSTEM__LITTLEFS
    LittleFS.begin();
    WebServer.http->serveStatic("/", LittleFS, "/webapp/", "max-age=3600").setDefaultFile("v" VERSION ".html");
#else
    SPIFFS.begin();
    WebServer.http->serveStatic("/", SPIFFS, "/webapp/", "max-age=3600").setDefaultFile("v" VERSION ".html");
#endif // BOARD_BUILD__FILESYSTEM__LITTLEFS
#endif // F_VERBOSE
    WebServer.http->on("/", WebRequestMethod::HTTP_HEAD, &onHeadRoot);
}

void WebAppExtension::onHeadRoot(AsyncWebServerRequest *request)
{
    AsyncWebServerResponse *response = request->beginResponse(t_http_codes::HTTP_CODE_OK);
    response->addHeader("Access-Control-Allow-Methods", "HEAD");
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}

#endif // EXTENSION_WEBAPP
