#if EXTENSION_WEBAPP

#include "extensions/WebAppExtension.h"

#include "services/ExtensionsService.h"
#include "services/WebServerService.h"

#include <HTTPClient.h>
#include <LittleFS.h>

void WebAppExtension::configure()
{
    File file{LittleFS.open(path.data())};
    length = file.size();
    if (length == 0U)
    {
        ESP_LOGE(name.data(), "File not found"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
#if EXTENSION_STATUSLED
        Extensions.StatusLed().error();
#endif // EXTENSION_STATUSLED
    }
    else
    {
        snprintf(etag.data(), etag.size(), R"("%08lx")", static_cast<uint32_t>(file.getLastWrite()));
        file.close();
        WebServer.http->on(AsyncURIMatcher::exact("/"), WebRequestMethod::HTTP_GET, &onGetRoot);
        WebServer.http->on(AsyncURIMatcher::exact("/"), WebRequestMethod::HTTP_HEAD, &onHeadRoot);
    }
}

void WebAppExtension::onGetRoot(AsyncWebServerRequest *request)
{
    const AsyncWebHeader *_etag{request->getHeader("If-None-Match")};
    if (_etag != nullptr && strcmp(_etag->value().c_str(), etag.data()) == 0)
    {
        AsyncWebServerResponse *response{request->beginResponse(t_http_codes::HTTP_CODE_NOT_MODIFIED)};
        response->addHeader("ETag", etag.data(), false);
        response->setContentLength(length);
        request->send(response);
    }
    else
    {
        AsyncWebServerResponse *response{request->beginResponse(LittleFS, path.data(), "text/html")};
        response->addHeader("Content-Encoding", "gzip", false);
        response->addHeader("ETag", etag.data(), false);
        request->send(response);
    }
}

void WebAppExtension::onHeadRoot(AsyncWebServerRequest *request)
{
    AsyncWebServerResponse *response{request->beginResponse(t_http_codes::HTTP_CODE_OK)};
    response->addHeader("Access-Control-Allow-Methods", "HEAD", false);
    response->addHeader("Access-Control-Allow-Origin", "*", false);
    response->addHeader("ETag", etag.data(), false);
    response->setContentLength(length);
    request->send(response);
}

#endif // EXTENSION_WEBAPP
