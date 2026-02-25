#pragma once

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "modules/ServiceModule.h"

#include <DNSServer.h> // NOLINT(misc-include-cleaner)
#include <WiFiMulti.h>
#include <span>

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

    [[nodiscard]] static std::span<const uint8_t> certificates() noexcept;

    static ConnectivityService &getInstance();
};

extern ConnectivityService &Connectivity; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
