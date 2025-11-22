#include <esp_crt_bundle.h>
#include <esp_sntp.h>
#include <esp_wifi.h>
#include <ESPmDNS.h>
#include <Preferences.h>

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
#endif // PIN_SW1
#ifdef PIN_SW2
    pinMode(PIN_SW2, INPUT_PULLUP);
#endif // PIN_SW2
    esp_crt_bundle_set(Certificates::x509_crt_bundle_start, Certificates::x509_crt_bundle_end - Certificates::x509_crt_bundle_start);
    WiFi.onEvent(&onConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(&onDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    WiFi.onEvent(&onIPv4, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent(&onIPv6, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP6);
    WiFi.onEvent(&onScan, WiFiEvent_t::ARDUINO_EVENT_WIFI_SCAN_DONE);
    WiFi.setHostname(HOSTNAME);
    WiFi.mode(wifi_mode_t::WIFI_MODE_STA);
#ifdef WIFI_COUNTRY
    esp_wifi_set_country_code(WIFI_COUNTRY, false);
#else
    Preferences Storage;
    Storage.begin(_name.data(), true);
    if (Storage.isKey("country"))
    {
        const String country = Storage.getString("country");
        Storage.end();
        ESP_LOGD(name, "country %s", country.c_str());
        esp_wifi_set_country_code(country.c_str(), true);
    }
    else
    {
        Storage.end();
    }
#endif // WIFI_COUNTRY
    WiFi.enableIPv6();
    vault();
#if defined(PIN_SW1) || defined(PIN_SW2)
    if (esp_sleep_get_wakeup_cause() == esp_sleep_source_t::ESP_SLEEP_WAKEUP_UNDEFINED && buttonCheck())
    {
        hotspot();
    }
    else
#endif // defined(PIN_SW1) || defined(PIN_SW2)
    {
        multi->run();
    }
    configTzTime(TIME_ZONE, "pool.ntp.org", "time.cloudflare.com", "time.nist.gov");
}

void ConnectivityService::ready()
{
#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_rssi");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::device_class] = "signal_strength";
        component[HomeAssistantAbbreviations::entity_category] = "diagnostic";
        component[HomeAssistantAbbreviations::expire_after] = UINT8_MAX;
        component[HomeAssistantAbbreviations::force_update] = true;
        component[HomeAssistantAbbreviations::name] = "Wi-Fi signal";
        component[HomeAssistantAbbreviations::object_id] = HOSTNAME "_" + id;
        component[HomeAssistantAbbreviations::platform] = "sensor";
        component[HomeAssistantAbbreviations::state_class] = "measurement";
        component[HomeAssistantAbbreviations::state_topic] = topic;
        component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[HomeAssistantAbbreviations::unit_of_measurement] = "dBm";
        component[HomeAssistantAbbreviations::value_template] = "{{value_json.rssi}}";
    }
#endif // EXTENSION_HOMEASSISTANT
}

void ConnectivityService::handle()
{
    if (dnsServer && WiFi.getMode() != wifi_mode_t::WIFI_MODE_STA)
    {
        dnsServer->processNextRequest();
    }
    else if (millis() - lastMillis > UINT16_MAX)
    {
        lastMillis = millis();
        if (WiFi.isConnected())
        {
            transmit();
        }
        else
        {
            multi->run();
        }
    }
}

void ConnectivityService::vault()
{
    JsonDocument doc;
    Preferences Storage;
    Storage.begin(name);
    if (Storage.isKey("Wi-Fi"))
    {
        const size_t _length = Storage.getBytesLength("Wi-Fi");
        uint8_t *_buffer = new uint8_t[_length];
        Storage.getBytes("Wi-Fi", _buffer, _length);
        deserializeJson(doc, _buffer, _length);
        delete[] _buffer;
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
    if (doc[WIFI_SSID].isUnbound())
    {
        doc[WIFI_SSID] = WIFI_KEY;
    }
    const size_t length = measureJson(doc);
    uint8_t *buffer = new uint8_t[length + 1];
    serializeJson(doc, reinterpret_cast<char *>(buffer), length + 1);
    Storage.putBytes("Wi-Fi", buffer, length + 1);
    Storage.end();
    delete[] buffer;
    multi = std::make_unique<WiFiMulti>();
    for (const JsonPairConst &pair : doc.as<JsonObjectConst>())
    {
        multi->addAP(pair.key().c_str(), pair.value().as<const char *>());
    }
}

void ConnectivityService::hotspot()
{
    ESP_LOGV(name, "initializing Wi-Fi hotspot");
    WiFi.mode(wifi_mode_t::WIFI_MODE_AP);
    WiFi.softAP(NAME);
    if (!dnsServer)
    {
        dnsServer = std::make_unique<DNSServer>();
        dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
        dnsServer->start(53, "*", WiFi.softAPIP());
    }
#if EXTENSION_WEBAPP
    ESP_LOGD(name, "user interface @ http://%s", WiFi.softAPIP().toString());
    ESP_LOGI(name, "awaiting Wi-Fi config, please connect to the Wi-Fi hotspot...");
#endif // EXTENSION_WEBAPP
}

void ConnectivityService::connect(const char *const ssid, const char *const key)
{
    if (WiFi.getMode() == wifi_mode_t::WIFI_MODE_AP)
    {
        WiFi.mode(wifi_mode_t::WIFI_MODE_APSTA);
    }
    multi = std::make_unique<WiFiMulti>();
    multi->addAP(ssid, key);
    WiFi.disconnect();
    multi->run();
    if (WiFi.waitForConnectResult() == wl_status_t::WL_CONNECTED)
    {
        JsonDocument doc;
        doc["event"] = "connected";
        Device.transmit(doc, name, false);
        if (WiFi.getMode() != wifi_mode_t::WIFI_MODE_STA)
        {
            ESP_LOGD(name, "terminating Wi-Fi hotspot");
            dnsServer.reset();
            WiFi.mode(wifi_mode_t::WIFI_MODE_STA);
        }
    }
}

#if defined(PIN_SW1) || defined(PIN_SW2)
bool ConnectivityService::buttonCheck() const
{
#ifdef PIN_SW1
    if (digitalRead(PIN_SW1) == LOW)
    {
        ESP_LOGV(name, "power button held at startup");
        return true;
    }
#endif // PIN_SW1
#ifdef PIN_SW2
    if (digitalRead(PIN_SW2) == LOW)
    {
        ESP_LOGV(name, "mode button held at startup");
        return true;
    }
#endif // PIN_SW2
    return false;
}
#endif // defined(PIN_SW1) || defined(PIN_SW2)

#ifdef DNS4
void ConnectivityService::setDns()
{
    esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    {
        esp_netif_dns_info_t dns;
        dns.ip.type = ESP_IPADDR_TYPE_V4;
        dns.ip.u_addr.ip4.addr = ipaddr_addr(DNS4);
        if (!esp_netif_set_dns_info(netif, esp_netif_dns_type_t::ESP_NETIF_DNS_FALLBACK, &dns))
        {
            ESP_LOGD(name, "DNS " DNS4);
        }
    }
}
#endif // DNS4

void ConnectivityService::onConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    ESP_LOGD(Connectivity.name, "connected");
    ESP_LOGV(Connectivity.name, "%d dBm", WiFi.RSSI());
    ESP_LOGI(Connectivity.name, HOSTNAME ".local");
#ifndef WIFI_COUNTRY
    char country[3];
    if (!esp_wifi_get_country_code(country))
    {
        Preferences Storage;
        Storage.begin(_name.data());
        if (strcmp(country, "01"))
        {
            Storage.putString("country", country);
        }
        else if (Storage.isKey("country"))
        {
            Storage.remove("country");
        }
        Storage.end();
    }
#endif // WIFI_COUNTRY
}

void ConnectivityService::onDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Connectivity.routable = false;
    ESP_LOGI(Connectivity.name, "disconnected");
    ESP_LOGD(Connectivity.name, "%s", WiFi.disconnectReasonName(static_cast<wifi_err_reason_t>(info.wifi_sta_disconnected.reason)));
}

void ConnectivityService::onIPv4(WiFiEvent_t event, WiFiEventInfo_t info)
{
    ESP_LOGI(Connectivity.name, "IPv4 %s", WiFi.localIP().toString().c_str());
    if (!Connectivity.routable)
    {
        onRoutable();
    }
}

void ConnectivityService::onIPv6(WiFiEvent_t event, WiFiEventInfo_t info)
{
    const char *const ipv6 = WiFi.globalIPv6().toString().c_str();
    if (strcmp(ipv6, ""))
    {
        ESP_LOGI(Connectivity.name, "IPv6 %s", ipv6);
        if (!Connectivity.routable)
        {
            onRoutable();
        }
    }
}

void ConnectivityService::onRoutable()
{
    Connectivity.routable = true;
    if (!Connectivity.mDNS && MDNS.begin(HOSTNAME))
    {
        Connectivity.mDNS = true;
        MDNS.setInstanceName(NAME);
#if EXTENSION_WEBAPP
        MDNS.addService("http", "tcp", 80);
#endif // EXTENSION_WEBAPP
#if EXTENSION_OTA && defined(OTA_KEY)
        MDNS.enableArduino(3232, true);
#elif EXTENSION_OTA
        MDNS.enableArduino(3232);
#endif // EXTENSION_OTA && defined(OTA_KEY)
    }
    timeval tv;
    sntp_sync_time(&tv);
}

void ConnectivityService::onScan(WiFiEvent_t event, WiFiEventInfo_t info)
{
    const int16_t n = WiFi.scanComplete();
    if (n > 0)
    {
        JsonDocument doc;
        JsonArray scan = doc["scan"].to<JsonArray>();
        for (uint8_t i = 0; i < n; ++i)
        {
            JsonObject _scan = scan.add<JsonObject>();
            _scan["encrypted"] = (bool)WiFi.encryptionType(i);
            _scan["rssi"] = WiFi.RSSI(i);
            _scan["ssid"] = WiFi.SSID(i);
        }
        Device.transmit(doc, _name.data(), false);
    }
}

void ConnectivityService::transmit()
{
    JsonDocument doc;
    doc["host"] = HOSTNAME ".local";
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
    doc["ssid"] = WiFi.SSID();
    Device.transmit(doc, name);
}

void ConnectivityService::receiverHook(const JsonDocument doc, const char *const source)
{
    // Connect
    if (doc["ssid"].is<const char *>())
    {
        connect(doc["ssid"].as<const char *>(), doc["key"].is<const char *>() ? doc["key"].as<const char *>() : nullptr);
    }
    // Scan
    if (doc["action"].is<const char *>() && !strcmp(doc["action"].as<const char *>(), "scan"))
    {
        ESP_LOGD(name, "scanning for Wi-Fi networks...");
        WiFi.scanNetworks(true);
    }
}

ConnectivityService &ConnectivityService::getInstance()
{
    static ConnectivityService instance;
    return instance;
}

ConnectivityService &Connectivity = Connectivity.getInstance();
