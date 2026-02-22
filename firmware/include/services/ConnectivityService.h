#pragma once

#include "config/constants.h"
#include "modules/ServiceModule.h"

#include <DNSServer.h>
#include <WiFiMulti.h>

class ConnectivityService final : public ServiceModule
{
private:
    static constexpr std::string_view _name = "Connectivity";

    explicit ConnectivityService() : ServiceModule(_name.data()) {};

    bool mDNS = false;
    bool pending = false;
    bool routable = false;

    unsigned long lastMillis = 0;

    std::unique_ptr<DNSServer> dns = nullptr;

    WiFiMulti multi;

    void initStation();
    void initHotspot();
    void connect(const char *ssid, const char *key);
    void transmit();

    static void onConnected(WiFiEvent_t event, WiFiEventInfo_t info);
    static void onDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);
    static void onIPv4(WiFiEvent_t event, WiFiEventInfo_t info);
    static void onIPv6(WiFiEvent_t event, WiFiEventInfo_t info);
    static void onRoutable();
    static void onScan(WiFiEvent_t event, WiFiEventInfo_t info);

public:
    static constexpr std::string_view userAgent = "Frekvens/" VERSION " (ESP32; +https://github.com/VIPnytt/Frekvens)";

    void configure();
    void begin();
    void handle();

    void onReceive(JsonObjectConst payload, const char *source) override;

    static ConnectivityService &getInstance();
};

extern ConnectivityService &Connectivity;

namespace Certificates
{
extern const uint8_t x509_crt_bundle_start[] asm("_binary_" BOARD_BUILD__EMBED_FILES__X509_CRT_BUNDLE "_start");
extern const uint8_t x509_crt_bundle_end[] asm("_binary_" BOARD_BUILD__EMBED_FILES__X509_CRT_BUNDLE "_end");
} // namespace Certificates
