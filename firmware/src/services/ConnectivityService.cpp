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

void ConnectivityService::configure()
{
#ifdef PIN_SW1
    pinMode(PIN_SW1, INPUT_PULLUP);
#endif // PIN_SW1
#ifdef PIN_SW2
    pinMode(PIN_SW2, INPUT_PULLUP);
#endif // PIN_SW2
    esp_crt_bundle_set(Certificates::x509_crt_bundle_start, Certificates::x509_crt_bundle_end - Certificates::x509_crt_bundle_start);
    WiFi.enableIPv6();
    WiFi.setHostname(HOSTNAME);
    WiFi.mode(wifi_mode_t::WIFI_MODE_STA);
    WiFi.onEvent(&onConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(&onDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    WiFi.onEvent(&onIPv4, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent(&onIPv6, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP6);
    WiFi.onEvent(&onScan, WiFiEvent_t::ARDUINO_EVENT_WIFI_SCAN_DONE);
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
#if defined(PIN_SW1) || defined(PIN_SW2)
#if defined(PIN_SW1) && defined(PIN_SW2)
    if (esp_sleep_get_wakeup_cause() == esp_sleep_source_t::ESP_SLEEP_WAKEUP_UNDEFINED && (digitalRead(PIN_SW1) == LOW || digitalRead(PIN_SW2) == LOW))
#elif defined(PIN_SW1)
    if (esp_sleep_get_wakeup_cause() == esp_sleep_source_t::ESP_SLEEP_WAKEUP_UNDEFINED && digitalRead(PIN_SW1) == LOW)
#elif defined(PIN_SW2)
    if (esp_sleep_get_wakeup_cause() == esp_sleep_source_t::ESP_SLEEP_WAKEUP_UNDEFINED && digitalRead(PIN_SW2) == LOW)
#endif // defined(PIN_SW1) && defined(PIN_SW2)
    {
        initHotspot();
    }
    else
#endif // defined(PIN_SW1) || defined(PIN_SW2)
    {
        initStation();
    }
    configTzTime(TIME_ZONE, "pool.ntp.org", "time.cloudflare.com", "time.nist.gov");
}

void ConnectivityService::begin()
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
    if (dns && WiFi.getMode() != wifi_mode_t::WIFI_MODE_STA)
    {
        dns->processNextRequest();
    }
    else if (millis() - lastMillis > UINT16_MAX)
    {
        lastMillis = millis();
        if (WiFi.isConnected())
        {
            transmit();
        }
        else if (WiFi.getMode() == wifi_mode_t::WIFI_MODE_STA)
        {
            multi.run();
        }
    }
}

void ConnectivityService::initStation()
{
    JsonDocument doc;
    Preferences Storage;
    Storage.begin(name);
    if (Storage.isKey("Wi-Fi"))
    {
        const size_t _length = Storage.getBytesLength("Wi-Fi");
        std::vector<char> _buffer(_length);
        Storage.getBytes("Wi-Fi", _buffer.data(), _length);
        deserializeJson(doc, _buffer.data(), _length);
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
    std::vector<uint8_t> buffer(length + 1);
    serializeJson(doc, reinterpret_cast<char *>(buffer.data()), length + 1);
    Storage.putBytes("Wi-Fi", buffer.data(), length + 1);
    Storage.end();
    for (const JsonPairConst &credentials : doc.as<JsonObjectConst>())
    {
        multi.addAP(credentials.key().c_str(), credentials.value().as<const char *>());
    }
    multi.run();
}

void ConnectivityService::initHotspot()
{
    ESP_LOGV(name, "initializing Wi-Fi hotspot");
    WiFi.mode(wifi_mode_t::WIFI_MODE_AP);
    WiFi.softAP(NAME);
    if (!dns)
    {
        dns = std::make_unique<DNSServer>();
        dns->setErrorReplyCode(DNSReplyCode::NoError);
        dns->start(53, "*", WiFi.softAPIP());
    }
#if EXTENSION_WEBAPP
    ESP_LOGD(name, "web interface @ http://%s", WiFi.softAPIP().toString());
    ESP_LOGI(name, "awaiting Wi-Fi configuration, please connect to the Wi-Fi hotspot...");
#endif // EXTENSION_WEBAPP
}

void ConnectivityService::connect(const char *const ssid, const char *const key)
{
    if (WiFi.getMode() == wifi_mode_t::WIFI_MODE_AP)
    {
        WiFi.mode(wifi_mode_t::WIFI_MODE_APSTA);
    }
    multi.setStrictMode(true);
    multi.APlistClean();
    multi.addAP(ssid, key);
    multi.run();
}

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
    if (Connectivity.mDNS)
    {
        MDNS.end();
        Connectivity.mDNS = false;
    }
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
    if (WiFi.getMode() != wifi_mode_t::WIFI_MODE_STA)
    {
        JsonDocument doc;
        doc["event"] = "connected";
        Device.transmit(doc, Connectivity.name, false);
        ESP_LOGD(Connectivity.name, "terminating Wi-Fi hotspot");
        Connectivity.dns.reset();
        WiFi.mode(wifi_mode_t::WIFI_MODE_STA);
    }
    if (!Connectivity.mDNS && MDNS.begin(HOSTNAME))
    {
        Connectivity.mDNS = true;
        MDNS.setInstanceName(NAME);
#if EXTENSION_ALEXA || (EXTENSION_OTA && !defined(OTA_KEY)) || EXTENSION_RESTFUL || EXTENSION_WEBAPP
        MDNS.addService("http", "tcp", 80);
#endif
#if EXTENSION_WEBSOCKET
        MDNS.addService("ws", "tcp", 80);
#endif // EXTENSION_WEBSOCKET
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
            std::vector<uint8_t> buf(len);
            Storage.getBytes("saved", buf.data(), len);
            Storage.end();
            JsonDocument _saved;
            if (!deserializeJson(_saved, buf.data(), len))
            {
                JsonArray saved = doc["saved"].to<JsonArray>();
                for (const JsonPairConst &credentials : _saved.as<JsonObjectConst>())
                {
                    saved.add(credentials.key());
                }
            }
        }
        else
        {
            Storage.end();
        }
    }
    doc["ssid"] = WiFi.SSID();
    Device.transmit(doc, name);
}

void ConnectivityService::onReceive(const JsonDocument doc, const char *const source)
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
