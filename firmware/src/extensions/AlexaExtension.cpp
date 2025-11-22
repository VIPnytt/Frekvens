#include "config/constants.h"

#if EXTENSION_ALEXA

#include <HTTPClient.h>

#include "extensions/AlexaExtension.h"
#include "services/DisplayService.h"
#include "services/WebServerService.h"

AlexaExtension *Alexa = nullptr;

AlexaExtension::AlexaExtension() : ExtensionModule("Alexa")
{
    Alexa = this;
}

void AlexaExtension::ready()
{
    fauxmo.createServer(false);
    fauxmo.setPort(80);
    fauxmo.addDevice(NAME);
    fauxmo.onSetState(&onSetState);

    WebServer.http->on("/api", WebRequestMethod::HTTP_POST, [](AsyncWebServerRequest *request) {}, nullptr, &onSet);
    WebServer.http->on("/api/2WLEDHardQrI3WHYTHoMcXHgEspsM8ZZRpSKtBQr/lights", WebRequestMethod::HTTP_GET, &onGet);
    WebServer.http->on("/api/2WLEDHardQrI3WHYTHoMcXHgEspsM8ZZRpSKtBQr/lights/1", WebRequestMethod::HTTP_GET, &onGet);
    WebServer.http->on("/api/2WLEDHardQrI3WHYTHoMcXHgEspsM8ZZRpSKtBQr/lights/1/state", WebRequestMethod::HTTP_PUT, [](AsyncWebServerRequest *request) {}, nullptr, &onSet);
    WebServer.http->on("/description.xml", WebRequestMethod::HTTP_GET, &onGet);

    fauxmo.setState(NAME, Display.getPower(), static_cast<unsigned char>(Display.getBrightness()));
    fauxmo.enable(true);
}

void AlexaExtension::handle()
{
    fauxmo.handle();
}

void AlexaExtension::onSetState(unsigned char deviceId, const char *deviceName, bool state, unsigned char value)
{
    if (!strcmp(deviceName, NAME))
    {
        Display.setBrightness(static_cast<uint8_t>(value), Alexa->name);
        Display.setPower(state, Alexa->name);
    }
}

void AlexaExtension::onGet(AsyncWebServerRequest *request)
{
    if (!Alexa->fauxmo.process(request->client(), true, request->url(), ""))
    {
        request->send(t_http_codes::HTTP_CODE_INTERNAL_SERVER_ERROR);
    }
}

void AlexaExtension::onSet(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    if (!Alexa->fauxmo.process(request->client(), false, request->url(), (char *)data))
    {
        request->send(t_http_codes::HTTP_CODE_BAD_REQUEST);
    }
}

void AlexaExtension::transmitterHook(const JsonDocument &doc, const char *const source)
{
    // Display: Brightness
    // Display: Power
    if (!strcmp(source, Display.name) && (doc["brightness"].is<uint8_t>() || doc["power"].is<bool>()))
    {
        fauxmo.setState(NAME, doc["power"].is<bool>() ? doc["power"].as<bool>() : Display.getPower(), static_cast<unsigned char>(doc["brightness"].is<uint8_t>() ? doc["brightness"].as<uint8_t>() : Display.getBrightness()));
    }
}

#endif // EXTENSION_ALEXA
