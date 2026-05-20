#if EXTENSION_RESTFUL

#include "extensions/RestfulExtension.h"

#include "services/DeviceService.h"
#include "services/WebServerService.h"

#include <HTTPClient.h>
#include <vector>

void RestfulExtension::begin()
{
    WebServer.http->on(AsyncURIMatcher::dir("/restful/"), WebRequestMethod::HTTP_GET, &onGet);
    WebServer.http->on(
        AsyncURIMatcher::dir("/restful/"), WebRequestMethod::HTTP_PATCH, &WebServerService::onEmpty, nullptr, &onPatch);
}

void RestfulExtension::onGet(AsyncWebServerRequest *request)
{
    const String module{request->url().substring(prefixLength)};
    const JsonObjectConst transmits{Device.getTransmits()};
    if (module.isEmpty())
    {
        const size_t length{measureJson(transmits)};
        std::vector<char> content(length + 1U);
        serializeJson(transmits, content.data(), length + 1U);
        request->send(t_http_codes::HTTP_CODE_OK, "application/json", content.data());
    }
    else if (transmits[module].is<JsonVariantConst>())
    {
        const size_t length{measureJson(transmits[module])};
        std::vector<char> content(length + 1U);
        serializeJson(transmits[module], content.data(), length + 1U);
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
    if (request->contentType() == "application/json")
    {
        if (index != 0U || len != total)
        {
            if (index == 0U)
            {
                buffer.resize(total);
            }
            std::copy_n(data, len, buffer.begin() + static_cast<std::ptrdiff_t>(index));
            if (index + len != total)
            {
                return;
            }
            data = buffer.data();
            len = buffer.size();
        }
        JsonDocument doc; // NOLINT(misc-const-correctness)
        if (deserializeJson(doc, data, len) == DeserializationError::Code::Ok)
        {
            const String destination{request->url().substring(prefixLength)};
            Device.receive(
                doc.as<JsonObjectConst>(), name, std::string_view{destination.c_str(), destination.length()});
            request->send(t_http_codes::HTTP_CODE_NO_CONTENT);
            return;
        }
    }
    request->send(t_http_codes::HTTP_CODE_BAD_REQUEST);
}

#endif // EXTENSION_RESTFUL
