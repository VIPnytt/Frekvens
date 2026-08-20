#if EXTENSION_RESTFUL

#include "extensions/RestfulExtension.h"

#include "services/DeviceService.h"
#include "services/WebServerService.h"

#include <HTTPClient.h>
#include <vector>

/**
 * @brief Registers the RESTful GET and PATCH endpoint handlers.
 */
void RestfulExtension::begin()
{
    WebServer.http.on(AsyncURIMatcher::dir("/restful/"), WebRequestMethod::HTTP_GET, &onGet);
    WebServer.http.on(
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

/**
 * @brief Processes a JSON PATCH request body and delivers it to the target device.
 *
 * Reassembles multipart request data before deserializing it. Successful JSON
 * requests are forwarded to the device and receive an HTTP 204 response.
 * Completed requests with another content type receive an HTTP 400 response.
 *
 * @param request Incoming HTTP request.
 * @param data Current request-body chunk.
 * @param len Number of bytes in the current chunk.
 * @param index Offset of the current chunk within the complete request body.
 * @param total Total request-body size in bytes.
 */
void RestfulExtension::onPatch(AsyncWebServerRequest *request, const uint8_t *data, size_t len, size_t index,
                               size_t total)
{
    const bool final{index + len == total};
    if (request->contentType() == "application/json")
    {
        if (index != 0U || len != total)
        {
            if (index == 0U)
            {
                buffer.resize(total);
            }
            std::copy_n(data, len, buffer.begin() + static_cast<std::ptrdiff_t>(index));
            if (!final)
            {
                return;
            }
            data = buffer.data();
            len = buffer.size();
        }
        JsonDocument doc{};
        if (deserializeJson(doc, data, len) == DeserializationError::Code::Ok)
        {
            const String destination{request->url().substring(prefixLength)};
            Device.receive(
                doc.as<JsonObjectConst>(), name, std::string_view{destination.c_str(), destination.length()});
            request->send(t_http_codes::HTTP_CODE_NO_CONTENT);
            return;
        }
    }
    else if (final)
    {
        request->send(t_http_codes::HTTP_CODE_BAD_REQUEST);
    }
}

#endif // EXTENSION_RESTFUL
