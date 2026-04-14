#if EXTENSION_WEBAPP

#include "extensions/WebAppExtension.h"

#include "services/WebServerService.h"

#include <HTTPClient.h>
#include <LittleFS.h>

void WebAppExtension::configure()
{
    if (!LittleFS.begin(false, "/littlefs", 1, "littlefs") || !LittleFS.exists("/webapp/index.html.gz"))
    {
        ESP_LOGE(name, "Filesystem Image not found"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    }
    WebServer.http->serveStatic("/", LittleFS, "/webapp/", "max-age=3600").setDefaultFile("index.html");
}

void WebAppExtension::begin()
{
    WebServer.http->on(AsyncURIMatcher::exact("/"), WebRequestMethod::HTTP_HEAD, &onHeadRoot);
}

void WebAppExtension::onHeadRoot(AsyncWebServerRequest *request)
{
    AsyncWebServerResponse *response = request->beginResponse(t_http_codes::HTTP_CODE_OK);
    response->addHeader("Access-Control-Allow-Methods", "HEAD");
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}

#endif // EXTENSION_WEBAPP
