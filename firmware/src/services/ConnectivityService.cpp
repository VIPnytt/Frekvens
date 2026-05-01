#include "services/ConnectivityService.h"

#include "extensions/HomeAssistantExtension.h" // NOLINT(misc-include-cleaner)
#include "services/DeviceService.h"

#include <ESPmDNS.h>
#include <esp_sntp.h>
#include <esp_wifi.h>
#include <nvs.h>

void ConnectivityService::configure()
{
#ifdef PIN_SW1
    pinMode(PIN_SW1, INPUT_PULLUP);
#endif // PIN_SW1
#ifdef PIN_SW2
    pinMode(PIN_SW2, INPUT_PULLUP);
#endif // PIN_SW2
    WiFiClass::setHostname(HOSTNAME);
    WiFiClass::mode(wifi_mode_t::WIFI_MODE_STA);
    WiFi.enableIPv6();
    WiFi.onEvent(&onConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(&onDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    WiFi.onEvent(&onIPv4, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent(&onIPv6, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP6);
    WiFi.onEvent(&onScan, WiFiEvent_t::ARDUINO_EVENT_WIFI_SCAN_DONE);
#ifdef WIFI_COUNTRY
    esp_wifi_set_country_code(WIFI_COUNTRY, false);
#else
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        std::array<char, 3> country{};
        size_t len{country.size()}; // NOLINT(cppcoreguidelines-init-variables)
        if (nvs_get_str(handle, "country", country.data(), &len) == ESP_OK)
        {
            nvs_close(handle);
            esp_wifi_set_country_code(country.data(), true);
        }
        else
        {
            nvs_close(handle);
        }
    }
#endif // WIFI_COUNTRY
#if defined(PIN_SW1) || defined(PIN_SW2)
#if defined(PIN_SW1) && defined(PIN_SW2)
    if (esp_sleep_get_wakeup_cause() == esp_sleep_source_t::ESP_SLEEP_WAKEUP_UNDEFINED &&
        (digitalRead(PIN_SW1) == LOW || digitalRead(PIN_SW2) == LOW))
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
        else if (WiFiClass::getMode() == wifi_mode_t::WIFI_MODE_STA)
        {
            multi.run();
        }
    }
}

void ConnectivityService::initStation()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    nvs_handle_t handle{};
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        size_t _len{0};
        if (nvs_get_blob(handle, "Wi-Fi", nullptr, &_len) == ESP_OK && _len > 0)
        {
            std::vector<char> _buffer(_len);
            if (nvs_get_blob(handle, "Wi-Fi", _buffer.data(), &_len) == ESP_OK)
            {
                nvs_close(handle);
                deserializeJson(doc, _buffer.data(), _len);
            }
            else
            {
                nvs_close(handle);
            }
        }
    }
    wifi_config_t config;
    if (esp_wifi_get_config(wifi_interface_t::WIFI_IF_STA, &config) == ESP_OK)
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        const char *const ssid_ptr = reinterpret_cast<const char *>(config.sta.ssid);
        const std::string_view ssid(ssid_ptr, strnlen(ssid_ptr, sizeof(config.sta.ssid)));
        if (!ssid.empty())
        {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            const char *const key_ptr = reinterpret_cast<const char *>(config.sta.password);
            const std::string_view key(key_ptr, strnlen(key_ptr, sizeof(config.sta.password)));
            doc[ssid] = key.length() ? key : nullptr;
        }
    }
    if (doc[WIFI_SSID].isUnbound())
    {
        doc[WIFI_SSID] = WIFI_KEY;
    }
    const size_t len = measureJson(doc);
    std::vector<uint8_t> buffer(len + 1);
    serializeJson(doc, reinterpret_cast<char *>(buffer.data()), len + 1);
    if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_blob(handle, "Wi-Fi", buffer.data(), len + 1);
        nvs_commit(handle);
        nvs_close(handle);
    }
    for (const JsonPairConst pair : doc.as<JsonObjectConst>())
    {
        multi.addAP(pair.key().c_str(), pair.value().as<const char *>());
    }
    multi.run();
}

void ConnectivityService::initHotspot()
{
    ESP_LOGV("Status", "initializing Wi-Fi hotspot"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    WiFiClass::mode(wifi_mode_t::WIFI_MODE_AP);
    WiFi.softAP(NAME);
    if (!dns)
    {
        dns = std::make_unique<DNSServer>();
        dns->setErrorReplyCode(DNSReplyCode::NoError);
        dns->start(53, "*", WiFi.softAPIP());
    }
#if EXTENSION_WEBAPP
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    ESP_LOGD("Wi-Fi", "web interface @ http://%s", WiFi.softAPIP().toString());
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    ESP_LOGI("Wi-Fi", "awaiting Wi-Fi configuration, please connect to the Wi-Fi hotspot...");
#endif // EXTENSION_WEBAPP
}

void ConnectivityService::connect(const char *ssid, const char *key) // NOLINT(bugprone-easily-swappable-parameters)
{
    if (WiFiClass::getMode() == wifi_mode_t::WIFI_MODE_AP)
    {
        WiFiClass::mode(wifi_mode_t::WIFI_MODE_APSTA);
    }
    multi.setStrictMode(true);
    multi.APlistClean();
    multi.addAP(ssid, key);
    multi.run();
}

void ConnectivityService::onConnected(WiFiEvent_t event,    // NOLINT(misc-unused-parameters)
                                      WiFiEventInfo_t info) // NOLINT(misc-unused-parameters)
{
    ESP_LOGD("Wi-Fi", "connected");           // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    ESP_LOGV("Wi-Fi", "%d dBm", WiFi.RSSI()); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    ESP_LOGI("Wi-Fi", HOSTNAME ".local");     // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
#ifndef WIFI_COUNTRY
    nvs_handle_t handle{};
    std::array<char, 3> country{};
    if (esp_wifi_get_country_code(country.data()) == ESP_OK &&
        nvs_open(Connectivity.name.data(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        if (strncmp(country.data(), "01", 2) == 0 ? nvs_erase_key(handle, "country") == ESP_OK
                                                  : nvs_set_str(handle, "country", country.data()) == ESP_OK)
        {
            nvs_commit(handle);
        }
        nvs_close(handle);
    }
#endif // WIFI_COUNTRY
}

void ConnectivityService::onDisconnected(WiFiEvent_t event, // NOLINT(misc-unused-parameters)
                                         WiFiEventInfo_t info)
{
    Connectivity.routable = false;
    if (Connectivity.mDNS)
    {
        MDNS.end();
        Connectivity.mDNS = false;
    }
    ESP_LOGI("Wi-Fi", "disconnected"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
                                       // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    ESP_LOGD(
        "Wi-Fi", "%s", WiFi.disconnectReasonName(static_cast<wifi_err_reason_t>(info.wifi_sta_disconnected.reason)));
}

void ConnectivityService::onIPv4(WiFiEvent_t event,    // NOLINT(misc-unused-parameters)
                                 WiFiEventInfo_t info) // NOLINT(misc-unused-parameters)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    ESP_LOGI("Wi-Fi", "IPv4 %s", WiFi.localIP().toString().c_str()); // NOLINT(hicpp-vararg)
    if (!Connectivity.routable)
    {
        onRoutable();
    }
}

void ConnectivityService::onIPv6(WiFiEvent_t event,    // NOLINT(misc-unused-parameters)
                                 WiFiEventInfo_t info) // NOLINT(misc-unused-parameters)
{
    const char *const ipv6 = WiFi.globalIPv6().toString().c_str();
    if (strcmp(ipv6, "") != 0)
    {
        ESP_LOGI("Wi-Fi", "IPv6 %s", ipv6); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        if (!Connectivity.routable)
        {
            onRoutable();
        }
    }
}

void ConnectivityService::onRoutable()
{
    Connectivity.routable = true;
    if (WiFiClass::getMode() != wifi_mode_t::WIFI_MODE_STA)
    {
        JsonDocument doc; // NOLINT(misc-const-correctness)
        doc["event"].set("connected");
        Device.transmit(doc.as<JsonObjectConst>(), Connectivity.name, false);
        ESP_LOGD("Wi-Fi", "terminating Wi-Fi hotspot"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        Connectivity.dns.reset();
        WiFiClass::mode(wifi_mode_t::WIFI_MODE_STA);
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
    timeval tv{};
    sntp_sync_time(&tv);
}

void ConnectivityService::onScan(WiFiEvent_t event,    // NOLINT(misc-unused-parameters)
                                 WiFiEventInfo_t info) // NOLINT(misc-unused-parameters)
{
    const int16_t count = WiFi.scanComplete();
    if (count > 0)
    {
        JsonDocument doc; // NOLINT(misc-const-correctness)
        JsonArray scan{doc["scan"].to<JsonArray>()};
        for (int16_t i = 0; i < count; ++i)
        {
            JsonObject _scan{scan.add<JsonObject>()};
            _scan["encrypted"].set(WiFi.encryptionType(i) != wifi_auth_mode_t::WIFI_AUTH_OPEN);
            _scan["rssi"].set(WiFi.RSSI(i));
            _scan["ssid"].set(WiFi.SSID(i));
        }
        Device.transmit(doc.as<JsonObjectConst>(), Connectivity.name, false);
    }
}

void ConnectivityService::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["host"].set(HOSTNAME ".local");
    doc["rssi"].set(WiFi.RSSI());
    {
        nvs_handle_t handle{};
        if (nvs_open(std::string(name).c_str(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
        {
            size_t len{0};
            if (nvs_get_blob(handle, "saved", nullptr, &len) == ESP_OK && len > 0)
            {
                std::vector<uint8_t> buffer(len);
                if (nvs_get_blob(handle, "Wi-Fi", &buffer, &len) == ESP_OK)
                {
                    nvs_close(handle);
                    JsonDocument _saved; // NOLINT(misc-const-correctness)
                    if (deserializeJson(_saved, buffer.data(), len) == DeserializationError::Code::Ok)
                    {
                        JsonArray saved{doc["saved"].to<JsonArray>()};
                        for (const JsonPairConst pair : _saved.as<JsonObjectConst>())
                        {
                            saved.add(pair.key());
                        }
                    }
                }
                else
                {
                    nvs_close(handle);
                }
            }
        }
    }
    doc["ssid"].set(WiFi.SSID());
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void ConnectivityService::onReceive(JsonObjectConst payload,
                                    std::string_view source) // NOLINT(misc-unused-parameters)
{
    // Connect
    if (payload["ssid"].is<const char *>())
    {
        connect(payload["ssid"].as<const char *>(),
                payload["key"].is<const char *>() ? payload["key"].as<const char *>() : nullptr);
    }
    // Action: Scan
    if (payload["action"].is<std::string_view>() && payload["action"].as<std::string_view>() == "scan")
    {
        ESP_LOGD("Wi-Fi", "scanning for networks..."); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        WiFi.scanNetworks(true);
    }
}

#if EXTENSION_HOMEASSISTANT
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void ConnectivityService::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique)
{
    topic.append(name);
    {
        const std::string id{std::string(name).append("_rssi")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::device_class].set("signal_strength");
        component[HomeAssistantAbbreviations::entity_category].set("diagnostic");
        component[HomeAssistantAbbreviations::expire_after].set(UINT8_MAX);
        component[HomeAssistantAbbreviations::force_update].set(true);
        component[HomeAssistantAbbreviations::name].set("Wi-Fi signal");
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        component[HomeAssistantAbbreviations::platform].set("sensor");
        component[HomeAssistantAbbreviations::state_class].set("measurement");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::unit_of_measurement].set("dBm");
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.rssi}}");
    }
}
#endif // EXTENSION_HOMEASSISTANT

ConnectivityService &ConnectivityService::getInstance()
{
    static ConnectivityService instance;
    return instance;
}

// NOLINTNEXTLINE(bugprone-throwing-static-initialization,cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables)
ConnectivityService &Connectivity = ConnectivityService::getInstance();
