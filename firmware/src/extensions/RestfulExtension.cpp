#include "config/constants.h"

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

void RestfulExtension::ready()
{
    WebServer.http->on("/restful/", WebRequestMethod::HTTP_GET, &onGet);
    for (const char *const _name : Device.getNames())
    {
        const std::string endpoint = std::string("/restful/").append(_name);
        WebServer.http->on(endpoint.c_str(), WebRequestMethod::HTTP_GET, &onGetModule);
        WebServer.http->on(endpoint.c_str(), WebRequestMethod::HTTP_PATCH, [](AsyncWebServerRequest *request) {}, nullptr, &onPatchModule);
    }
}

void RestfulExtension::onGet(AsyncWebServerRequest *request)
{
    const JsonDocument doc = Device.getTransmits();
    const size_t length = measureJson(doc);
    char *payload = new char[length + 1];
    serializeJson(doc, payload, length + 1);
    request->send(t_http_codes::HTTP_CODE_OK, "application/json", payload);
    delete[] payload;
}

void RestfulExtension::onGetModule(AsyncWebServerRequest *request)
{
    const JsonDocument doc = Device.getTransmits();
    const String module = request->url().substring(9);
    if (doc[module].is<JsonVariantConst>())
    {
        const size_t length = measureJson(doc[module]);
        char *payload = new char[length + 1];
        serializeJson(doc[module], payload, length + 1);
        request->send(t_http_codes::HTTP_CODE_OK, "application/json", payload);
        delete[] payload;
    }
    else
    {
        request->send(t_http_codes::HTTP_CODE_NO_CONTENT);
    }
}

void RestfulExtension::onPatchModule(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    JsonDocument doc;
    if (request->contentType() == "application/json" && !deserializeJson(doc, data))
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
