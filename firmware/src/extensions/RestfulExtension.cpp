#if EXTENSION_RESTFUL

#include "extensions/RestfulExtension.h"

#include "services/DeviceService.h"
#include "services/WebServerService.h"

#include <HTTPClient.h>
#include <vector>

RestfulExtension *Restful = nullptr;

RestfulExtension::RestfulExtension() : ExtensionModule("RESTful") { Restful = this; }

void RestfulExtension::begin()
{
    WebServer.http->on(AsyncURIMatcher::dir("/restful/"), WebRequestMethod::HTTP_GET, &onGet);
    WebServer.http->on(
        AsyncURIMatcher::dir("/restful/"), WebRequestMethod::HTTP_PATCH, &WebServer.onEmpty, nullptr, &onPatch);
}

void RestfulExtension::onGet(AsyncWebServerRequest *request)
{
    const String module = request->url().substring(prefixLength);
    const JsonObjectConst transmits = Device.getTransmits();
    if (module.isEmpty())
    {
        const size_t length = measureJson(transmits);
        std::vector<char> content(length + 1);
        serializeJson(transmits, content.data(), length + 1);
        request->send(t_http_codes::HTTP_CODE_OK, "application/json", content.data());
    }
    else if (transmits[module].is<JsonVariantConst>())
    {
        const size_t length = measureJson(transmits[module]);
        std::vector<char> content(length + 1);
        serializeJson(transmits[module], content.data(), length + 1);
        request->send(t_http_codes::HTTP_CODE_OK, "application/json", content.data());
    }
    else
    {
        request->send(t_http_codes::HTTP_CODE_NO_CONTENT);
    }
}

void RestfulExtension::onPatch(AsyncWebServerRequest *request, const uint8_t *data, size_t len, size_t index,
                               size_t total)
{
    JsonDocument doc;
    if (request->contentType() == "application/json" && !deserializeJson(doc, data, len))
    {
        Device.receive(doc.as<JsonObjectConst>(), Restful->name, request->url().substring(prefixLength).c_str());
        request->send(t_http_codes::HTTP_CODE_NO_CONTENT);
    }
    else
    {
        request->send(t_http_codes::HTTP_CODE_BAD_REQUEST);
    }
}

#endif // EXTENSION_RESTFUL
