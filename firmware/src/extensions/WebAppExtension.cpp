#include "config/constants.h"

#if EXTENSION_WEBAPP

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
    SPIFFS.begin();
#ifdef F_VERBOSE
    WebServer.http->serveStatic("/", SPIFFS, "/webapp/", "max-age=60").setDefaultFile("index.html");
#else
    WebServer.http->serveStatic("/", SPIFFS, "/webapp/", "max-age=3600").setDefaultFile("index.html");
#endif // F_VERBOSE
    WebServer.http->on("/", WebRequestMethod::HTTP_HEAD, &onHeadRoot);
}

void WebAppExtension::onHeadRoot(AsyncWebServerRequest *request)
{
    AsyncWebServerResponse *response = request->beginResponse(200); // OK
    response->addHeader("Access-Control-Allow-Methods", "HEAD");
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}

#endif // EXTENSION_WEBAPP
