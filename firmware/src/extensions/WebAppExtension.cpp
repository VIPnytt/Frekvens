#if EXTENSION_WEBAPP

#include "extensions/WebAppExtension.h"

#include "services/WebServerService.h"

#include <HTTPClient.h>
#include <LittleFS.h>

void WebAppExtension::configure()
{
    WebServer.http->on(AsyncURIMatcher::exact("/"), WebRequestMethod::HTTP_GET, &onGetRoot);
}

void WebAppExtension::begin()
{
    WebServer.http->on(AsyncURIMatcher::exact("/"), WebRequestMethod::HTTP_HEAD, &onHeadRoot);
}

void WebAppExtension::onGetRoot(AsyncWebServerRequest *request)
{
    request->send(request->beginResponse(LittleFS, "/webapp/index.html", "text/html"));
}

void WebAppExtension::onHeadRoot(AsyncWebServerRequest *request)
{
    AsyncWebServerResponse *response{request->beginResponse(t_http_codes::HTTP_CODE_OK)};
    response->addHeader("Access-Control-Allow-Methods", "HEAD");
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}

#endif // EXTENSION_WEBAPP
