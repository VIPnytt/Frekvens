#if EXTENSION_RESTFUL

#include <HTTPClient.h>
#include <vector>

#include "extensions/RestfulExtension.h"
#include "services/DeviceService.h"
#include "services/WebServerService.h"

RestfulExtension *Restful = nullptr;

RestfulExtension::RestfulExtension() : ExtensionModule("RESTful")
{
    Restful = this;
}

void RestfulExtension::begin()
{
    WebServer.http->on(AsyncURIMatcher::dir("/restful/"), WebRequestMethod::HTTP_GET, &onGet);
    WebServer.http->on(AsyncURIMatcher::dir("/restful/"), WebRequestMethod::HTTP_PATCH, &WebServer.onEmpty, nullptr, &onPatch);
}

void RestfulExtension::onGet(AsyncWebServerRequest *request)
{
    const String module = request->url().substring(prefixLength);
    const JsonDocument doc = Device.getTransmits();
    if (module.isEmpty())
    {
        const size_t length = measureJson(doc);
        std::vector<char> payload(length + 1);
        serializeJson(doc, payload.data(), length + 1);
        request->send(t_http_codes::HTTP_CODE_OK, "application/json", payload.data());
    }
    else if (doc[module].is<JsonVariantConst>())
    {
        const size_t length = measureJson(doc[module]);
        std::vector<char> payload(length + 1);
        serializeJson(doc[module], payload.data(), length + 1);
        request->send(t_http_codes::HTTP_CODE_OK, "application/json", payload.data());
    }
    else
    {
        request->send(t_http_codes::HTTP_CODE_NO_CONTENT);
    }
}

void RestfulExtension::onPatch(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    JsonDocument doc;
    if (request->contentType() == "application/json" && !deserializeJson(doc, data, len))
    {
        Device.receive(doc, Restful->name, request->url().substring(prefixLength).c_str());
        request->send(t_http_codes::HTTP_CODE_NO_CONTENT);
    }
    else
    {
        request->send(t_http_codes::HTTP_CODE_BAD_REQUEST);
    }
}

#endif // EXTENSION_RESTFUL
