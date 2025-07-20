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

    WebServer.http->on("/api", WebRequestMethod::HTTP_POST, [](AsyncWebServerRequest *request) {}, nullptr, &onSetApi);
    WebServer.http->on("/api/2WLEDHardQrI3WHYTHoMcXHgEspsM8ZZRpSKtBQr/lights", WebRequestMethod::HTTP_GET, &onGetApi);
    WebServer.http->on("/api/2WLEDHardQrI3WHYTHoMcXHgEspsM8ZZRpSKtBQr/lights/1", WebRequestMethod::HTTP_GET, &onGetApi);
    WebServer.http->on("/api/2WLEDHardQrI3WHYTHoMcXHgEspsM8ZZRpSKtBQr/lights/1/state", WebRequestMethod::HTTP_PUT, [](AsyncWebServerRequest *request) {}, nullptr, &onSetApi);
    WebServer.http->on("/description.xml", WebRequestMethod::HTTP_GET, &onGetApi);

    fauxmo.setState(NAME, Display.getPower(), static_cast<unsigned char>(Display.getGlobalBrightness()));
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
#ifdef F_INFO
        Serial.print(Alexa->name);
        Serial.println(state == Display.getPower() ? ": brightness" : ": power");
#endif
        Display.setGlobalBrightness(static_cast<uint8_t>(value));
        Display.setPower(state);
    }
}

void AlexaExtension::onGetApi(AsyncWebServerRequest *request)
{
    if (!Alexa->fauxmo.process(request->client(), true, request->url(), ""))
    {
        request->send(t_http_codes::HTTP_CODE_INTERNAL_SERVER_ERROR);
    }
}

void AlexaExtension::onSetApi(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
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
        fauxmo.setState(NAME, doc["power"].is<bool>() ? doc["power"].as<bool>() : Display.getPower(), static_cast<unsigned char>(doc["brightness"].is<uint8_t>() ? doc["brightness"].as<uint8_t>() : Display.getGlobalBrightness()));
    }
}

#endif // EXTENSION_ALEXA
