#include "config/constants.h"

#if EXTENSION_WEBAPP

#include <HTTPClient.h>
#include <LittleFS.h>

#include "extensions/WebAppExtension.h"
#include "services/DeviceService.h"

WebAppExtension *WebApp = nullptr;

WebAppExtension::WebAppExtension() : ExtensionModule("Web app")
{
    WebApp = this;
}

void WebAppExtension::setup()
{
    LittleFS.begin(false, "/littlefs", 1, "littlefs");
    if (!LittleFS.exists("/webapp/index.html.gz"))
    {
        ESP_LOGE(name, "front page not found");
    }
    WebServer.http->serveStatic("/", LittleFS, "/webapp/", "max-age=3600").setDefaultFile("index.html");
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
