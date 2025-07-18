#include "config/constants.h"

#if EXTENSION_RESTFUL

#include <vector>

#include "config/constants.h"
#include "extensions/RestfulExtension.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"
#include "services/FontsService.h"
#include "services/ModesService.h"
#include "services/NetworkService.h"
#include "services/WebServerService.h"

RestfulExtension *Restful = nullptr;

RestfulExtension::RestfulExtension() : ExtensionModule("RESTful")
{
    Restful = this;
}

void RestfulExtension::ready()
{
    WebServer.http->on("/api/", WebRequestMethod::HTTP_GET, &onGetApi);
    for (const char *const _name : Device.getNames())
    {
        const std::string _endpoint = std::string("/api/").append(_name);
        WebServer.http->on(_endpoint.c_str(), WebRequestMethod::HTTP_GET, &onGetApiModule);
        WebServer.http->on(_endpoint.c_str(), WebRequestMethod::HTTP_PATCH, [](AsyncWebServerRequest *request) {}, nullptr, &onSetApiModule);
    }
}

void RestfulExtension::onGetApi(AsyncWebServerRequest *request)
{
    const JsonDocument doc = Device.getTransmits();
    const size_t length = measureJson(doc);
    char *payload = new char[length + 1];
    serializeJson(doc, payload, length + 1);
    request->send(200, "application/json", payload); // OK
    delete[] payload;
}

void RestfulExtension::onGetApiModule(AsyncWebServerRequest *request)
{
    const JsonDocument doc = Device.getTransmits();
    const String module = request->url().substring(prefixLength);
    if (doc[module].is<JsonVariantConst>())
    {
        const size_t length = measureJson(doc);
        char *payload = new char[length + 1];
        serializeJson(doc, payload, length + 1);
        request->send(200, "application/json", payload); // OK
        delete[] payload;
    }
    else
    {
        request->send(204); // No Content
    }
}

void RestfulExtension::onSetApiModule(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    JsonDocument doc;
    if (request->contentType() == "application/json" && !deserializeJson(doc, data))
    {
        Device.receive(doc, Restful->name, request->url().substring(prefixLength).c_str());
        request->send(204); // No Content
    }
    else
    {
        request->send(400); // Bad Request
    }
}

#endif // EXTENSION_RESTFUL
