#if EXTENSION_ALEXA

#include "extensions/AlexaExtension.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "services/DisplayService.h"
#include "services/WebServerService.h"

#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <format>

void AlexaExtension::begin()
{
    WebServer.http->on(
        AsyncURIMatcher::exact("/api"), WebRequestMethod::HTTP_POST, &WebServerService::onEmpty, nullptr, &onPostApi);
    WebServer.http->on(AsyncURIMatcher::exact("/api/alexa/lights"), WebRequestMethod::HTTP_GET, &onGetLights);
    WebServer.http->on(AsyncURIMatcher::exact("/api/alexa/lights/1"), WebRequestMethod::HTTP_GET, &onGetLight);
    WebServer.http->on(AsyncURIMatcher::exact("/api/alexa/lights/1/state"),
                       WebRequestMethod::HTTP_PUT,
                       &WebServerService::onEmpty,
                       nullptr,
                       &onPutState);
    WebServer.http->on(AsyncURIMatcher::exact("/description.xml"), WebRequestMethod::HTTP_GET, &onGetDescription);
    upnp.onPacket(&onUpnp);
    upnp.listenMulticast(IPAddress(239U, 255U, 255U, 250U), 1900U);
}

std::string AlexaExtension::light()
{
    return std::format("{{"
                       R"("manufacturername":")" MANUFACTURER R"(",)"
                       R"("modelid":")" MODEL R"(",)"
                       R"("name":")" NAME R"(",)"
                       R"("state":{{)"
                       R"("bri":{},)"
                       R"("mode":"homeautomation",)"
                       R"("on":{},)"
                       R"("reachable":true)"
                       "}},"
                       R"("swversion":")" VERSION R"(",)"
                       R"("type":"Dimmable light",)"
                       R"("uniqueid":"00:{}:00:00")"
                       "}}",
                       min<uint8_t>(Display.getBrightness(), 254U),
                       Display.getPower(),
                       WiFi.macAddress().c_str());
}

std::pair<std::array<char, 17U>, std::array<char, 13U>> AlexaExtension::mac()
{
    std::array<uint8_t, 6U> _mac{};
    WiFi.macAddress(_mac.data());
    std::array<char, 17U> bridgeId{};
    std::array<char, 13U> uuidSuffix{};
    std::snprintf(bridgeId.data(),
                  bridgeId.size(),
                  "%02X%02X%02XFFFE%02X%02X%02X",
                  _mac[0U],
                  _mac[1U],
                  _mac[2U],
                  _mac[3U],
                  _mac[4U],
                  _mac[5U]);
    std::snprintf(uuidSuffix.data(),
                  uuidSuffix.size(),
                  "%02x%02x%02x%02x%02x%02x",
                  _mac[0U],
                  _mac[1U],
                  _mac[2U],
                  _mac[3U],
                  _mac[4U],
                  _mac[5U]);
    return std::pair<std::array<char, 17U>, std::array<char, 13U>>{bridgeId, uuidSuffix};
}

void AlexaExtension::onGetDescription(AsyncWebServerRequest *request)
{
    const std::pair<std::array<char, 17U>, std::array<char, 13U>> unique{mac()};
    request->send(t_http_codes::HTTP_CODE_OK,
                  "text/xml",
                  std::format(R"(<?xml version="1.0" encoding="UTF-8"?>)"
                              R"(<root xmlns="urn:schemas-upnp-org:device-1-0">)"
                              "<device>"
                              "<deviceType>urn:schemas-upnp-org:device:Basic:1</deviceType>"
                              "<friendlyName>" NAME " (" HOSTNAME ".local)</friendlyName>"
                              "<manufacturer>Royal Philips Electronics</manufacturer>"
                              "<manufacturerURL>http://www.philips.com</manufacturerURL>"
                              "<modelDescription>Philips hue Personal Wireless Lighting</modelDescription>"
                              "<modelName>Philips hue bridge 2015</modelName>"
                              "<modelNumber>BSB002</modelNumber>"
                              "<modelURL>http://www.meethue.com</modelURL>"
                              "<serialNumber>{}</serialNumber>"
                              "<UDN>uuid:2f402f80-da50-11e1-9b23-{}</UDN>"
                              "</device>"
                              "<specVersion>"
                              "<major>1</major>"
                              "<minor>0</minor>"
                              "</specVersion>"
                              "<URLBase>http://{}:80/</URLBase>"
                              "</root>",
                              unique.first.data(),
                              unique.second.data(),
                              WiFi.localIP().toString().c_str())
                      .c_str());
}

void AlexaExtension::onGetLight(AsyncWebServerRequest *request)
{
    request->send(t_http_codes::HTTP_CODE_OK, "application/json", light().c_str());
}

void AlexaExtension::onGetLights(AsyncWebServerRequest *request)
{
    request->send(t_http_codes::HTTP_CODE_OK, "application/json", std::format(R"({{"1":{}}})", light()).c_str());
}

void AlexaExtension::onMdns()
{
    MDNS.addService("hue", "tcp", 80U);
    MDNS.addServiceTxt("hue", "tcp", "bridgeid", static_cast<const char *>(mac().first.data()));
    MDNS.addServiceTxt("hue", "tcp", "modelid", "BSB002");
}

void AlexaExtension::onPutState(AsyncWebServerRequest *request, uint8_t *data, const size_t len, const size_t index,
                                const size_t total)
{
    JsonDocument doc;
    deserializeJson(doc, reinterpret_cast<const char *>(data), len);
    if (doc["bri"].is<uint8_t>())
    {
        const uint8_t brightness{doc["bri"].as<uint8_t>()};
        if (Display.getBrightness() != brightness)
        {
            Display.setBrightness(brightness);
        }
    }
    if (doc["on"].is<bool>())
    {
        const bool power{doc["on"].as<bool>()};
        if (Display.getPower() != power)
        {
            Display.setPower(power);
        }
    }
    request->send(t_http_codes::HTTP_CODE_OK,
                  "application/json",
                  std::format(R"([{{)"
                              R"("success":{{)"
                              R"("/lights/1/state/on":{})"
                              R"(}})"
                              R"(}}])",
                              Display.getPower())
                      .c_str());
}

void AlexaExtension::onPostApi(AsyncWebServerRequest *request,
                               uint8_t *data, // NOLINT(misc-unused-parameters)
                               size_t len,    // NOLINT(misc-unused-parameters)
                               size_t index,  // NOLINT(misc-unused-parameters)
                               size_t total)  // NOLINT(misc-unused-parameters)
{
    request->send(t_http_codes::HTTP_CODE_OK, "application/json", R"([{"success":{"username":"alexa"}}])");
}

void AlexaExtension::onUpnp(AsyncUDPPacket &packet)
{
    const std::string_view request{
        reinterpret_cast<const char *>(packet.data()),
        packet.length(),
    };
    if (request.find("M-SEARCH") != std::string_view::npos)
    {
        const bool root{request.find("upnp:rootdevice") != std::string_view::npos};
        const std::pair<std::array<char, 17U>, std::array<char, 13U>> unique{mac()};
        const std::string response{std::format("HTTP/1.1 200 OK\r\n"
                                               "EXT:\r\n"
                                               "LOCATION: http://{}:80/description.xml\r\n"
                                               "SERVER: FreeRTOS/6.0.5, UPnP/1.0, IpBridge/1.17.0\r\n"
                                               "ST: {}\r\n"
                                               "USN: uuid:2f402f80-da50-11e1-9b23-{}{}\r\n"
                                               "hue-bridgeid: {}\r\n"
                                               "\r\n",
                                               WiFi.localIP().toString().c_str(),
                                               root ? "upnp:rootdevice" : "urn:schemas-upnp-org:device:basic:1",
                                               unique.second.data(),
                                               root ? "::upnp:rootdevice" : "",
                                               unique.first.data())};
        upnp.writeTo(reinterpret_cast<const uint8_t *>(response.data()),
                     response.size(),
                     packet.remoteIP(),
                     packet.remotePort());
    }
}

#endif // EXTENSION_ALEXA
