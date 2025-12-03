#pragma once

#include <DNSServer.h>
#include <WiFiMulti.h>

#include "config/constants.h"
#include "modules/ServiceModule.h"

class ConnectivityService : public ServiceModule
{
private:
    static constexpr std::string_view _name = "Connectivity";

    ConnectivityService() : ServiceModule(_name.data()) {};

    bool
        mDNS = false,
        pending = false,
        routable = false;

    unsigned long lastMillis = 0;

    std::unique_ptr<DNSServer> dns;

    WiFiMulti multi;

    void initStation();
    void initHotspot();
    void connect(const char *const ssid, const char *const key);
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

    void onReceive(const JsonDocument doc, const char *const source) override;

    static ConnectivityService &getInstance();
};

extern ConnectivityService &Connectivity;

namespace Certificates
{
    extern const uint8_t x509_crt_bundle_start[] asm("_binary_" BOARD_BUILD__EMBED_FILES__X509_CRT_BUNDLE "_start");
    extern const uint8_t x509_crt_bundle_end[] asm("_binary_" BOARD_BUILD__EMBED_FILES__X509_CRT_BUNDLE "_end");
}
