#if EXTENSION_ALEXA

#include "extensions/AlexaExtension.h"

#include "services/DisplayService.h"
#include "services/ExtensionsService.h"
#include "services/WebServerService.h"

#include <HTTPClient.h>

void AlexaExtension::begin()
{
    fauxmo.createServer(false);
    fauxmo.setPort(80);
    fauxmo.addDevice(NAME);
    fauxmo.onSetState(&onSetState);

    WebServer.http->on(
        AsyncURIMatcher::exact("/api"), WebRequestMethod::HTTP_POST, &WebServerService::onEmpty, nullptr, &onSet);
    WebServer.http->on(AsyncURIMatcher::exact("/api/2WLEDHardQrI3WHYTHoMcXHgEspsM8ZZRpSKtBQr/lights"),
                       WebRequestMethod::HTTP_GET,
                       &onGet);
    WebServer.http->on(AsyncURIMatcher::exact("/api/2WLEDHardQrI3WHYTHoMcXHgEspsM8ZZRpSKtBQr/lights/1"),
                       WebRequestMethod::HTTP_GET,
                       &onGet);
    WebServer.http->on(AsyncURIMatcher::exact("/api/2WLEDHardQrI3WHYTHoMcXHgEspsM8ZZRpSKtBQr/lights/1/state"),
                       WebRequestMethod::HTTP_PUT,
                       &WebServerService::onEmpty,
                       nullptr,
                       &onSet);
    WebServer.http->on(AsyncURIMatcher::exact("/description.xml"), WebRequestMethod::HTTP_GET, &onGet);

    fauxmo.setState(NAME, Display.getPower(), static_cast<unsigned char>(Display.getBrightness()));
    fauxmo.enable(true);
}

void AlexaExtension::handle() { fauxmo.handle(); }

void AlexaExtension::onSetState(unsigned char deviceId, // NOLINT(misc-unused-parameters)
                                const char *deviceName, bool state, unsigned char value)
{
    if (strcmp(deviceName, NAME) == 0)
    {
        Display.setBrightness(static_cast<uint8_t>(value));
        Display.setPower(state);
    }
}

void AlexaExtension::onGet(AsyncWebServerRequest *request)
{
    if (!fauxmo.process(request->client(), true, request->url(), ""))
    {
        request->send(t_http_codes::HTTP_CODE_INTERNAL_SERVER_ERROR);
    }
}

void AlexaExtension::onSet(AsyncWebServerRequest *request, uint8_t *data, size_t len,
                           size_t index, // NOLINT(misc-unused-parameters)
                           size_t total) // NOLINT(misc-unused-parameters)
{
    if (!fauxmo.process(
            request->client(),
            false,
            request->url(),
            String(reinterpret_cast<const char *>(data), len))) // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
    {
        request->send(t_http_codes::HTTP_CODE_BAD_REQUEST);
    }
}

void AlexaExtension::onTransmit(JsonObjectConst payload, std::string_view source)
{
    // Display: Brightness
    // Display: Power
    if (source == Display.name && (payload["brightness"].is<uint8_t>() || payload["power"].is<bool>()))
    {
        fauxmo.setState(NAME,
                        payload["power"].is<bool>() ? payload["power"].as<bool>() : Display.getPower(),
                        static_cast<unsigned char>(payload["brightness"].is<uint8_t>()
                                                       ? payload["brightness"].as<uint8_t>()
                                                       : Display.getBrightness()));
    }
}

#endif // EXTENSION_ALEXA
