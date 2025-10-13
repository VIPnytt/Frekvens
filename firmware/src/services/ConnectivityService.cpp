#include <esp_sntp.h>
#include <esp_wifi.h>
#include <ESPmDNS.h>
#include <Preferences.h>

#include "extensions/BuildExtension.h"
#include "extensions/HomeAssistantExtension.h"
#include "handlers/TextHandler.h"
#include "services/ConnectivityService.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/WebServerService.h"

void ConnectivityService::setup()
{
#ifdef PIN_SW1
    pinMode(PIN_SW1, INPUT_PULLUP);
#endif
#ifdef PIN_SW2
    pinMode(PIN_SW2, INPUT_PULLUP);
#endif
    WiFi.onEvent(&onConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent(&onDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    WiFi.onEvent(&onScan, WiFiEvent_t::ARDUINO_EVENT_WIFI_SCAN_DONE);
    WiFi.setHostname(HOSTNAME);
    WiFi.mode(wifi_mode_t::WIFI_MODE_STA);
    WiFi.enableIpV6();
#if defined(PIN_SW1) || defined(PIN_SW2)
    if (!vault() || buttonCheck())
#else
    if (!vault())
#endif // defined(PIN_SW1) || defined(PIN_SW2)
    {
#ifdef F_VERBOSE
        Serial.printf("%s: initializing Wi-Fi hotspot\n", name);
#endif // F_VERBOSE
        hotspot();
    }
    else
    {
        multi->run();
    }
#if defined(TIME_ZONE_POSIX) && defined(NTP1) && defined(NTP2) && defined(NTP3)
    configTzTime(TIME_ZONE_POSIX, NTP1, NTP2, NTP3);
#elif defined(TIME_ZONE_POSIX) && defined(NTP1) && defined(NTP2)
    configTzTime(TIME_ZONE_POSIX, NTP1, NTP2);
#elif defined(TIME_ZONE_POSIX) && defined(NTP1)
    configTzTime(TIME_ZONE_POSIX, NTP1);
#elif defined(TIME_ZONE_POSIX)
    configTzTime(TIME_ZONE_POSIX, "pool.ntp.org", "time.nist.gov", "time.cloudflare.com");
#endif // defined(TIME_ZONE_POSIX) && defined(NTP1) && defined(NTP2) && defined(NTP3)
}

void ConnectivityService::ready()
{
#if EXTENSION_BUILD
#ifdef DOMAIN
    (*Build->config)[Config::env][__STRING(DOMAIN)] = DOMAIN;
#endif // DOMAIN
    (*Build->config)[Config::env][__STRING(HOSTNAME)] = HOSTNAME;
#ifdef TIME_ZONE_IANA
    (*Build->config)[Config::env][__STRING(TIME_ZONE_IANA)] = TIME_ZONE_IANA;
#endif // TIME_ZONE_IANA
#ifdef DNS1
    (*Build->config)[Config::h][__STRING(DNS1)] = DNS1;
#ifdef DNS2
    (*Build->config)[Config::h][__STRING(DNS2)] = DNS2;
#endif // DNS2
#ifdef DNS3
    (*Build->config)[Config::h][__STRING(DNS3)] = DNS3;
#endif // DNS3
#endif // DNS1
#ifdef NTP1
    (*Build->config)[Config::h][__STRING(NTP1)] = NTP1;
#ifdef NTP2
    (*Build->config)[Config::h][__STRING(NTP2)] = NTP2;
#ifdef NTP3
    (*Build->config)[Config::h][__STRING(NTP3)] = NTP3;
#endif // NTP3
#endif // NTP2
#endif // NTP1
#ifdef WIFI_SSID
    (*Build->config)[Config::h][__STRING(WIFI_SSID)] = WIFI_SSID;
#ifdef WIFI_KEY
    (*Build->config)[Config::h][__STRING(WIFI_KEY)] = "REDACTED";
#endif // WIFI_KEY
#endif // WIFI_SSID
#ifdef WIFI_SSID_HOTSPOT
    (*Build->config)[Config::h][__STRING(WIFI_SSID_HOTSPOT)] = WIFI_SSID_HOTSPOT;
#endif // WIFI_SSID_HOTSPOT
#ifdef WIFI_KEY_HOTSPOT
    (*Build->config)[Config::h][__STRING(WIFI_KEY_HOTSPOT)] = "REDACTED";
#endif // WIFI_KEY_HOTSPOT
#endif // EXTENSION_BUILD

#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_rssi");
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::device_class] = "signal_strength";
        component[Abbreviations::entity_category] = "diagnostic";
        component[Abbreviations::expire_after] = UINT8_MAX;
        component[Abbreviations::force_update] = true;
        component[Abbreviations::name] = "Wi-Fi signal";
        component[Abbreviations::object_id] = HOSTNAME "_" + id;
        component[Abbreviations::platform] = "sensor";
        component[Abbreviations::state_class] = "measurement";
        component[Abbreviations::state_topic] = topic;
        component[Abbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[Abbreviations::unit_of_measurement] = "dBm";
        component[Abbreviations::value_template] = "{{value_json.rssi}}";
    }
#endif // EXTENSION_HOMEASSISTANT
}

void ConnectivityService::handle()
{
    if (dnsServer && WiFi.getMode() != wifi_mode_t::WIFI_MODE_STA)
    {
        dnsServer->processNextRequest();
    }
    else if (!WiFi.isConnected() && millis() - lastMillis > UINT16_MAX)
    {
        lastMillis = millis();
        multi->run();
    }
    else if (pending || (WiFi.isConnected() && millis() - _lastMillis > UINT16_MAX))
    {
        _lastMillis = millis();
        pending = false;
        transmit();
    }
}

bool ConnectivityService::vault()
{
    JsonDocument doc;
    Preferences Storage;
    Storage.begin(name);
    if (Storage.isKey("saved"))
    {
        const size_t _len = Storage.getBytesLength("saved");
        uint8_t *_buf = new uint8_t[_len];
        Storage.getBytes("saved", _buf, _len);
        deserializeJson(doc, _buf, _len);
        delete[] _buf;
    }
    wifi_config_t config;
    if (!esp_wifi_get_config(wifi_interface_t::WIFI_IF_STA, &config))
    {
        const char *_ssid = reinterpret_cast<const char *>(config.sta.ssid);
        const std::string_view ssid(_ssid, strnlen(_ssid, sizeof(config.sta.ssid)));
        if (ssid.length())
        {
            const char *_key = reinterpret_cast<const char *>(config.sta.password);
            const std::string_view key(_key, strnlen(_key, sizeof(config.sta.password)));
            doc[ssid] = key.length() ? key : nullptr;
        }
    }
#ifdef WIFI_SSID
    if (doc[WIFI_SSID].isUnbound())
    {
#ifdef WIFI_KEY
        doc[WIFI_SSID] = WIFI_KEY;
#else
        doc[WIFI_SSID] = nullptr;
#endif // WIFI_KEY
    }
#endif // WIFI_SSID
    const size_t len = measureJson(doc);
    uint8_t *buf = new uint8_t[len + 1];
    serializeJson(doc, reinterpret_cast<char *>(buf), len + 1);
    Storage.putBytes("saved", buf, len + 1);
    Storage.end();
    delete[] buf;
    multi = std::make_unique<WiFiMulti>();
    for (const JsonPairConst &pair : doc.as<JsonObjectConst>())
    {
        multi->addAP(pair.key().c_str(), pair.value().as<const char *>());
    }
#ifdef F_VERBOSE
    const bool loaded = doc.size();
    if (!loaded)
    {
        Serial.printf("%s: Wi-Fi config not found\n", name);
    }
    return loaded;
#else
    return doc.size();
#endif // F_VERBOSE
}

void ConnectivityService::hotspot()
{
    WiFi.mode(wifi_mode_t::WIFI_MODE_AP);
#if defined(WIFI_SSID_HOTSPOT) && defined(WIFI_KEY_HOTSPOT)
    WiFi.softAP(WIFI_SSID_HOTSPOT, WIFI_KEY_HOTSPOT);
#elif defined(WIFI_SSID_HOTSPOT)
    WiFi.softAP(WIFI_SSID_HOTSPOT);
#elif defined(WIFI_KEY_HOTSPOT)
    WiFi.softAP(NAME, WIFI_KEY_HOTSPOT);
#else
    WiFi.softAP(NAME);
#endif // defined(WIFI_SSID_HOTSPOT) && defined(WIFI_KEY_HOTSPOT)
    if (!dnsServer)
    {
        dnsServer = std::make_unique<DNSServer>();
        dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
        dnsServer->start(53, "*", WiFi.softAPIP());
    }
#ifdef F_INFO
#ifdef WIFI_SSID_HOTSPOT
    Serial.printf("%s: hotspot SSID " WIFI_SSID_HOTSPOT "\n", name);
#else
    Serial.printf("%s: hotspot SSID " NAME "\n", name);
#endif // WIFI_SSID_HOTSPOT
#endif // F_INFO
#if defined(WIFI_KEY_HOTSPOT) && defined(F_VERBOSE)
    Serial.printf("%s: hotspot key " WIFI_KEY_HOTSPOT "\n", name);
#endif
#if EXTENSION_WEBAPP && defined(F_DEBUG)
    Serial.printf("%s: user interface @ http://%s\n", name, WiFi.softAPIP().toString());
#endif // EXTENSION_WEBAPP && defined(F_DEBUG)
#ifdef F_INFO
    Serial.printf("%s: awaiting Wi-Fi config, please connect to the Wi-Fi hotspot...\n", name);
#endif // F_INFO
}

void ConnectivityService::scan()
{
    int16_t n = WiFi.scanComplete();
    if (n == WIFI_SCAN_FAILED)
    {
#ifdef F_DEBUG
        Serial.printf("%s: scanning for Wi-Fi networks...\n", name);
        WiFi.scanNetworks(true, true);
#else
        WiFi.scanNetworks(true);
#endif // F_DEBUG
    }
#ifdef F_VERBOSE
    else if (n == WIFI_SCAN_RUNNING)
    {
        Serial.printf("%s: scan already ongoing...\n", name);
    }
#endif // F_VERBOSE
}

void ConnectivityService::connect(const char *const ssid, const char *const key)
{
    if (WiFi.getMode() == wifi_mode_t::WIFI_MODE_AP)
    {
        WiFi.mode(wifi_mode_t::WIFI_MODE_APSTA);
    }
#ifdef F_DEBUG
    Serial.printf("%s: SSID %s\n", name, ssid);
#endif // F_DEBUG
    WiFi.begin(ssid, key);
    if (WiFi.waitForConnectResult() == wl_status_t::WL_CONNECTED)
    {
        JsonDocument doc;
        doc["event"] = "connected";
        Device.transmit(doc, name, false);
#ifdef F_DEBUG
        if (WiFi.getMode() != wifi_mode_t::WIFI_MODE_STA)
        {
            Serial.printf("%s: terminating Wi-Fi hotspot\n", name);
        }
#endif // F_DEBUG
        WiFi.mode(wifi_mode_t::WIFI_MODE_STA);
        vault();
        dnsServer.reset();
    }
}

void ConnectivityService::disconnect()
{
    lastMillis = millis();
    WiFi.disconnect();
}

#if defined(PIN_SW1) || defined(PIN_SW2)
bool ConnectivityService::buttonCheck()
{
#ifdef F_VERBOSE
    if (esp_sleep_get_wakeup_cause() == esp_sleep_source_t::ESP_SLEEP_WAKEUP_UNDEFINED)
    {
#ifdef PIN_SW1
        if (digitalRead(PIN_SW1) == LOW)
        {
            Serial.printf("%s: power button held at startup\n", name);
            return true;
        }
#endif // PIN_SW1
#ifdef PIN_SW2
        if (digitalRead(PIN_SW2) == LOW)
        {
            Serial.printf("%s: mode button held at startup\n", name);
            return true;
        }
#endif // PIN_SW2
    }
#elif defined(PIN_SW1) && defined(PIN_SW2)
    return esp_sleep_get_wakeup_cause() == esp_sleep_source_t::ESP_SLEEP_WAKEUP_UNDEFINED && (digitalRead(PIN_SW1) == LOW || digitalRead(PIN_SW2) == LOW);
#elif defined(PIN_SW1)
    return esp_sleep_get_wakeup_cause() == esp_sleep_source_t::ESP_SLEEP_WAKEUP_UNDEFINED && digitalRead(PIN_SW1) == LOW;
#elif defined(PIN_SW2)
    return esp_sleep_get_wakeup_cause() == esp_sleep_source_t::ESP_SLEEP_WAKEUP_UNDEFINED && digitalRead(PIN_SW2) == LOW;
#endif // F_VERBOSE
    return false;
}
#endif // defined(PIN_SW1) || defined(PIN_SW2)

#if defined(DNS1) || defined(DNS2) || defined(DNS3)
void ConnectivityService::setDns()
{
    esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
#ifdef DNS1
    {
        esp_netif_dns_info_t dns;
        dns.ip.type = ESP_IPADDR_TYPE_V4;
        dns.ip.u_addr.ip4.addr = ipaddr_addr(DNS1);
#ifdef F_DEBUG
        if (!esp_netif_set_dns_info(netif, esp_netif_dns_type_t::ESP_NETIF_DNS_MAIN, &dns))
        {
            Serial.printf("%s: DNS " DNS1 "\n", name);
        }
#else
        esp_netif_set_dns_info(netif, esp_netif_dns_type_t::ESP_NETIF_DNS_MAIN, &dns);
#endif // F_DEBUG
    }
#endif // DNS1

#ifdef DNS2
    {
        esp_netif_dns_info_t dns;
        dns.ip.type = ESP_IPADDR_TYPE_V4;
        dns.ip.u_addr.ip4.addr = ipaddr_addr(DNS2);
#ifdef F_DEBUG
        if (!esp_netif_set_dns_info(netif, esp_netif_dns_type_t::ESP_NETIF_DNS_BACKUP, &dns))
        {
            Serial.printf("%s: DNS " DNS2 "\n", name);
        }
#else
        esp_netif_set_dns_info(netif, esp_netif_dns_type_t::ESP_NETIF_DNS_BACKUP, &dns);
#endif // F_DEBUG
    }
#endif // DNS2

#ifdef DNS3
    {
        esp_netif_dns_info_t dns;
        dns.ip.type = ESP_IPADDR_TYPE_V4;
        dns.ip.u_addr.ip4.addr = ipaddr_addr(DNS3);
#ifdef F_DEBUG
        if (!esp_netif_set_dns_info(netif, esp_netif_dns_type_t::ESP_NETIF_DNS_FALLBACK, &dns))
        {
            Serial.printf("%s: DNS " DNS3 "\n", name);
        }
#else
        esp_netif_set_dns_info(netif, esp_netif_dns_type_t::ESP_NETIF_DNS_FALLBACK, &dns);
#endif // F_DEBUG
    }
#endif // DNS3
}
#endif // defined(DNS1) || defined(DNS2) || defined(DNS3)

void ConnectivityService::onConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
#ifdef F_VERBOSE
    Serial.printf("%s: connected, %d dBm @ %s\n", Connectivity.name, WiFi.RSSI(), WiFi.SSID().c_str());
#elif defined(F_DEBUG)
    Serial.printf("%s: connected, %d dBm\n", Connectivity.name, WiFi.RSSI());
#elif defined(F_INFO)
    Serial.printf("%s: connected\n", Connectivity.name);
#endif // F_VERBOSE
    if (!Connectivity.mDNS)
    {
        Connectivity.mDNS = MDNS.begin(HOSTNAME);
        if (Connectivity.mDNS)
        {
            MDNS.setInstanceName(NAME);
#if EXTENSION_WEBAPP
            MDNS.addService("http", "tcp", 80);
#endif // EXTENSION_WEBAPP
#if EXTENSION_OTA && (defined(OTA_KEY_HASH) || defined(OTA_KEY))
            MDNS.enableArduino(3232, true);
#elif EXTENSION_OTA
            MDNS.enableArduino(3232, false);
#endif // EXTENSION_OTA && (defined(OTA_KEY_HASH) || defined(OTA_KEY))
        }
    }
    const String
        _ipv4 = WiFi.localIP().toString(),
        _ipv6 = WiFi.localIPv6().toString();
    if (Connectivity.IPv4 != _ipv4 || Connectivity.IPv6 != _ipv6)
    {
        Connectivity.IPv4 = _ipv4;
        Connectivity.IPv6 = _ipv6;
#ifdef F_INFO
        if (Connectivity.IPv4 != IPAddress().toString())
        {
            Serial.printf("%s: IPv4 %s\n", Connectivity.name, Connectivity.IPv4.c_str());
        }
        if (Connectivity.IPv6 != IPv6Address().toString())
        {
            Serial.printf("%s: IPv6 %s\n", Connectivity.name, Connectivity.IPv6.c_str());
        }
#endif // F_INFO
#ifdef F_INFO
        Serial.printf("%s: domain %s\n", Connectivity.name, domain.data());
#endif // F_INFO
    }
#if defined(DNS1) || defined(DNS2) || defined(DNS3)
    Connectivity.setDns();
#endif // defined(DNS1) || defined(DNS2) || defined(DNS3)
    timeval tv;
    sntp_sync_time(&tv);
    Connectivity.pending = true;
}

void ConnectivityService::onDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Connectivity.lastMillis = millis();
#ifdef F_DEBUG
    Serial.printf("%s: disconnected, %s\n", Connectivity.name, WiFi.disconnectReasonName(static_cast<wifi_err_reason_t>(info.wifi_sta_disconnected.reason)));
#elif defined(F_INFO)
    Serial.printf("%s: disconnected\n", Connectivity.name);
#endif // F_DEBUG
    WiFi.reconnect();
}

void ConnectivityService::onScan(WiFiEvent_t event, WiFiEventInfo_t info)
{
#ifdef F_VERBOSE
    Serial.printf("%s: scan complete\n", _name.data());
#endif // F_VERBOSE
    int16_t n = WiFi.scanComplete();
    if (n > 0)
    {
        JsonDocument doc;
        JsonArray scan = doc["scan"].to<JsonArray>();
        for (uint8_t i = 0; i < n; ++i)
        {
            JsonObject _scan = scan.add<JsonObject>();
#ifdef F_DEBUG
            _scan["bssid"] = WiFi.BSSIDstr(i);
            _scan["channel"] = WiFi.channel(i);
#endif // F_DEBUG
            _scan["encrypted"] = (bool)WiFi.encryptionType(i);
            _scan["rssi"] = WiFi.RSSI(i);
#ifdef F_VERBOSE
            if (WiFi.SSID(i).length())
            {
                _scan["ssid"] = WiFi.SSID(i);
            }
#else
            _scan["ssid"] = WiFi.SSID(i);
#endif // F_VERBOSE
        }
        Device.transmit(doc, _name.data(), false);
    }
    WiFi.scanDelete();
}

void ConnectivityService::transmit()
{
    JsonDocument doc;

#ifdef F_VERBOSE
    doc["broadcast"] = WiFi.broadcastIP().toString();
#endif // F_VERBOSE

#ifdef F_DEBUG
    doc["bssid"] = WiFi.BSSIDstr();
    JsonArray dns = doc["dns"].to<JsonArray>();
    for (esp_netif_dns_type_t dnsType : {
             esp_netif_dns_type_t::ESP_NETIF_DNS_MAIN,
             esp_netif_dns_type_t::ESP_NETIF_DNS_BACKUP,
#ifdef DNS3
             esp_netif_dns_type_t::ESP_NETIF_DNS_FALLBACK,
#endif // DNS3
         })
    {
        String _dns = WiFi.dnsIP(dnsType).toString();
        if (_dns != IPAddress().toString())
        {
            dns.add(_dns);
        }
    }
#endif // F_VERBOSE

    doc["domain"] = domain;

#ifdef F_VERBOSE
    doc["gateway"] = WiFi.gatewayIP().toString();
#endif // F_VERBOSE

#ifdef F_DEBUG
    if (Connectivity.IPv4 != IPAddress().toString())
    {
        doc["ipv4"] = Connectivity.IPv4;
    }
    if (Connectivity.IPv6 != IPv6Address().toString())
    {
        doc["ipv6"] = Connectivity.IPv6;
    }
    doc["hostname"] = HOSTNAME;
#endif // F_DEBUG

#ifdef F_VERBOSE
#ifdef WIFI_SSID_HOTSPOT
    doc["hotspot"] = WIFI_SSID_HOTSPOT;
#else
    doc["hotspot"] = NAME;
#endif // WIFI_SSID_HOTSPOT
    doc["mac"] = WiFi.macAddress();
#endif // F_VERBOSE

    doc["rssi"] = WiFi.RSSI();
    {
        Preferences Storage;
        Storage.begin(name, true);
        if (Storage.isKey("saved"))
        {
            const size_t len = Storage.getBytesLength("saved");
            uint8_t *buf = new uint8_t[len];
            Storage.getBytes("saved", buf, len);
            Storage.end();
            JsonDocument _saved;
            if (!deserializeJson(_saved, buf, len))
            {
                JsonArray saved = doc["saved"].to<JsonArray>();
                for (const JsonPairConst &pair : _saved.as<JsonObjectConst>())
                {
                    saved.add(pair.key());
                }
            }
            delete[] buf;
        }
        else
        {
            Storage.end();
        }
    }
    if (WiFi.SSID().length())
    {
        doc["ssid"] = WiFi.SSID();
    }
#ifdef F_VERBOSE
    if (WiFi.subnetMask().toString() != IPAddress().toString())
    {
        doc["subnet"] = WiFi.subnetMask().toString();
    }
#endif // F_VERBOSE

    Device.transmit(doc, name);
}

void ConnectivityService::receiverHook(const JsonDocument doc)
{
    // Connect
    if (doc["ssid"].is<const char *>())
    {
        connect(doc["ssid"].as<const char *>(), doc["key"].is<const char *>() ? doc["key"].as<const char *>() : nullptr);
    }
    // Scan
    if (doc["action"].is<const char *>() && !strcmp(doc["action"].as<const char *>(), "scan"))
    {
        scan();
    }
}

ConnectivityService &ConnectivityService::getInstance()
{
    static ConnectivityService instance;
    return instance;
}

ConnectivityService &Connectivity = Connectivity.getInstance();
