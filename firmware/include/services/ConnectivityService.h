#pragma once

#include "config/constants.h"
#include "modules/ServiceModule.h"

#include <DNSServer.h>
#include <WiFiMulti.h>

class ConnectivityService : public ServiceModule
{
private:
    static constexpr std::string_view _name = "Connectivity";

    ConnectivityService() : ServiceModule(_name.data()) {};

    bool
        mDNS = false,
        pending = false;

    unsigned long
        lastMillis = 0,
        _lastMillis = 0;

    String
        IPv4,
        IPv6;

    std::unique_ptr<DNSServer> dnsServer;
    std::unique_ptr<WiFiMulti> multi;

    static inline bool scanning = false;

    bool vault();
    void hotspot();
    void scan();
    void connect(const char *const ssid, const char *const key);
    void transmit();
#if defined(PIN_SW1) || defined(PIN_SW2)
    bool buttonCheck();
#endif
#if defined(DNS1) || defined(DNS2) || defined(DNS3)
    void setDns();
#endif

    static void onConnected(WiFiEvent_t event, WiFiEventInfo_t info);
    static void onDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);
    static void onScan(WiFiEvent_t event, WiFiEventInfo_t info);

public:
#ifdef DOMAIN
    static constexpr std::string_view domain = HOSTNAME DOMAIN;
#else
    static constexpr std::string_view domain = HOSTNAME ".local";
#endif // DOMAIN
    static constexpr std::string_view userAgent = "Frekvens/" VERSION " (ESP32; +https://github.com/VIPnytt/Frekvens)";

    void setup();
    void ready();
    void handle();
    void disconnect();

    void receiverHook(const JsonDocument doc) override;

    static ConnectivityService &getInstance();
};

extern ConnectivityService &Connectivity;
