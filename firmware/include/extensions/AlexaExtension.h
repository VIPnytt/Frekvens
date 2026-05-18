#pragma once

#if EXTENSION_ALEXA

#include "modules/ExtensionModule.h"

#include <AsyncUDP.h>
#include <ESPAsyncWebServer.h>

class AlexaExtension final : public ExtensionModule
{
private:
    static constexpr std::string_view name{"Alexa"};

    static inline AsyncUDP upnp{}; // NOLINT(bugprone-throwing-static-initialization,cert-err58-cpp)

    [[nodiscard]] static std::string light();
    [[nodiscard]] static std::pair<std::array<char, 17U>, std::array<char, 13U>> mac();
    static void onGetDescription(AsyncWebServerRequest *request);
    static void onGetLight(AsyncWebServerRequest *request);
    static void onGetLights(AsyncWebServerRequest *request);
    static void onPostApi(AsyncWebServerRequest *request, const uint8_t *data, size_t len, size_t index, size_t total);
    static void onPutState(AsyncWebServerRequest *request, const uint8_t *data, size_t len, size_t index, size_t total);
    static void onUpnp(AsyncUDPPacket &packet);

public:
    explicit AlexaExtension() : ExtensionModule(name) {};

    void begin() override;

    static void onMdns();
};

#endif // EXTENSION_ALEXA
