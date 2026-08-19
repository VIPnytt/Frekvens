#include "services/ConnectivityService.h"

#include "extensions/AlexaExtension.h"
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
    WiFi.onEvent(&onConnected, arduino_event_id_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(&onDisconnected, arduino_event_id_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    WiFi.onEvent(&onIPv4, arduino_event_id_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent(&onIPv6, arduino_event_id_t::ARDUINO_EVENT_WIFI_STA_GOT_IP6);
    WiFi.onEvent(&onScan, arduino_event_id_t::ARDUINO_EVENT_WIFI_SCAN_DONE);
#ifdef WIFI_COUNTRY
    esp_wifi_set_country_code(WIFI_COUNTRY, false);
#else
    nvs_handle_t handle{};
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        std::array<char, 3U> country{};
        size_t length{country.size()};
        if (nvs_get_str(handle, "country", country.data(), &length) == ESP_OK && length == country.size())
        {
            esp_wifi_set_country_code(country.data(), true);
        }
        nvs_close(handle);
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
    configTzTime(TIME_ZONE_POSIX, "1.pool.ntp.org", "2.pool.ntp.org", "3.pool.ntp.org");
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
    JsonDocument doc{};
    nvs_handle_t handle{};
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
    {
        size_t _length{0U};
        if (nvs_get_blob(handle, "Wi-Fi", nullptr, &_length) == ESP_OK && _length != 0U)
        {
            std::vector<char> _buffer(_length);
            if (nvs_get_blob(handle, "Wi-Fi", _buffer.data(), &_length) == ESP_OK)
            {
                deserializeJson(doc, _buffer.data(), _length);
            }
        }
        nvs_close(handle);
    }
    wifi_config_t config;
    if (esp_wifi_get_config(wifi_interface_t::WIFI_IF_STA, &config) == ESP_OK)
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        const char *const ssid_ptr{reinterpret_cast<const char *>(config.sta.ssid)};
        const std::string_view ssid(ssid_ptr, strnlen(ssid_ptr, sizeof(config.sta.ssid)));
        if (!ssid.empty())
        {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            const char *const key_ptr{reinterpret_cast<const char *>(config.sta.password)};
            const std::string_view key(key_ptr, strnlen(key_ptr, sizeof(config.sta.password)));
            doc[ssid].set(key.length() ? key : nullptr);
        }
    }
    if (doc[WIFI_SSID].isUnbound())
    {
        doc[WIFI_SSID].set(WIFI_KEY);
    }
    const size_t length{measureJson(doc) + 1U};
    std::vector<char> buffer(length);
    serializeJson(doc, reinterpret_cast<char *>(buffer.data()), length);
    if (nvs_open(name.data(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        nvs_set_blob(handle, "Wi-Fi", buffer.data(), length);
        nvs_commit(handle);
        nvs_close(handle);
    }
    for (const JsonPairConst pair : doc.as<JsonObjectConst>())
    {
        multi.addAP(pair.key().c_str(), pair.value().as<const char *>());
    }
    multi.run();
}

/**
 * @brief Starts the Wi-Fi access point and wildcard DNS service for hotspot operation.
 */
void ConnectivityService::initHotspot()
{
    ESP_LOGV(name.data(), "initializing Wi-Fi hotspot"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    WiFiClass::mode(wifi_mode_t::WIFI_MODE_AP);
    WiFi.softAP(NAME);
    if (!dns)
    {
        dns = std::make_unique<DNSServer>();
        dns->setErrorReplyCode(DNSReplyCode::NoError);
        dns->start(53U, "*", WiFi.softAPIP());
    }
#if EXTENSION_WEBAPP
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    ESP_LOGD(name.data(), "web interface @ http://192.168.4.1");
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    ESP_LOGI(name.data(), "awaiting Wi-Fi configuration, please connect to the Wi-Fi hotspot...");
#endif // EXTENSION_WEBAPP
}

/**
 * @brief Starts a strict connection attempt using the specified Wi-Fi credentials.
 *
 * @param ssid Wi-Fi network name.
 * @param key Wi-Fi network password.
 */
void ConnectivityService::connect(const char *ssid, const char *key)
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

/**
 * @brief Handles a successful Wi-Fi connection.
 *
 * Logs the connection status and signal strength, and stores the detected
 * Wi-Fi country code when no fixed country configuration is provided.
 *
 * @param event Wi-Fi event identifier.
 */
void ConnectivityService::onConnected(arduino_event_id_t event)
{
    ESP_LOGD(name.data(), "Wi-Fi connected");             // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    ESP_LOGV(name.data(), "RSSI %d dBm", WiFi.RSSI());    // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    ESP_LOGI(name.data(), "Hostname " HOSTNAME ".local"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
#ifndef WIFI_COUNTRY
    nvs_handle_t handle{};
    std::array<char, 3U> country{};
    if (esp_wifi_get_country_code(country.data()) == ESP_OK &&
        nvs_open(Connectivity.name.data(), nvs_open_mode_t::NVS_READWRITE, &handle) == ESP_OK)
    {
        if (strncmp(country.data(), "01", 2U) == 0 ? nvs_erase_key(handle, "country") == ESP_OK
                                                   : nvs_set_str(handle, "country", country.data()) == ESP_OK)
        {
            nvs_commit(handle);
        }
        nvs_close(handle);
    }
#endif // WIFI_COUNTRY
}

/**
 * @brief Handles Wi-Fi disconnection events.
 *
 * Marks the service as non-routable, stops mDNS when active, and logs the
 * disconnection reason.
 *
 * @param event Wi-Fi event identifier.
 * @param info Event-specific information containing the disconnection reason.
 */
void ConnectivityService::onDisconnected(arduino_event_id_t event, arduino_event_info_t info)
{
    Connectivity.routable = false;
    if (Connectivity.mdns)
    {
        MDNS.end();
        Connectivity.mdns = false;
    }
    ESP_LOGI(name.data(), "Wi-Fi disconnected"); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
                                                 // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    ESP_LOGD(name.data(),
             "Wi-Fi disconnect reason %s",
             WiFi.disconnectReasonName(static_cast<wifi_err_reason_t>(info.wifi_sta_disconnected.reason)));
}

/**
 * @brief Marks the service routable when the station receives an IPv4 address.
 *
 * @param event Wi-Fi event identifier.
 * @param info Event data containing the assigned IPv4 address.
 */
void ConnectivityService::onIPv4(arduino_event_id_t event, arduino_event_info_t info)
{
    if (WiFi.STA.hasIP())
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        ESP_LOGI(name.data(),
                 "IPv4 %u.%u.%u.%u",
                 static_cast<unsigned>(info.got_ip.ip_info.ip.addr & 0xFFU),
                 static_cast<unsigned>((info.got_ip.ip_info.ip.addr >> 8U) & 0xFFU),
                 static_cast<unsigned>((info.got_ip.ip_info.ip.addr >> 16U) & 0xFFU),
                 static_cast<unsigned>((info.got_ip.ip_info.ip.addr >> 24U) & 0xFFU));
        onRoutable();
    }
}

/**
 * @brief Handles assignment of a global IPv6 address.
 *
 * Logs the assigned address and marks the service routable when a global IPv6
 * address is available.
 *
 * @param event Wi-Fi event identifier.
 * @param info Event data containing the assigned IPv6 address.
 */
void ConnectivityService::onIPv6(arduino_event_id_t event, arduino_event_info_t info)
{
    if (WiFi.STA.hasGlobalIPv6())
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        ESP_LOGI(name.data(),
                 "IPv6 %x:%x:%x:%x:%x:%x:%x:%x",
                 static_cast<unsigned>((__builtin_bswap32(info.got_ip6.ip6_info.ip.addr[0]) >> 16U) & 0xFFFFU),
                 static_cast<unsigned>(__builtin_bswap32(info.got_ip6.ip6_info.ip.addr[0]) & 0xFFFFU),
                 static_cast<unsigned>((__builtin_bswap32(info.got_ip6.ip6_info.ip.addr[1]) >> 16U) & 0xFFFFU),
                 static_cast<unsigned>(__builtin_bswap32(info.got_ip6.ip6_info.ip.addr[1]) & 0xFFFFU),
                 static_cast<unsigned>((__builtin_bswap32(info.got_ip6.ip6_info.ip.addr[2]) >> 16U) & 0xFFFFU),
                 static_cast<unsigned>(__builtin_bswap32(info.got_ip6.ip6_info.ip.addr[2]) & 0xFFFFU),
                 static_cast<unsigned>((__builtin_bswap32(info.got_ip6.ip6_info.ip.addr[3]) >> 16U) & 0xFFFFU),
                 static_cast<unsigned>(__builtin_bswap32(info.got_ip6.ip6_info.ip.addr[3]) & 0xFFFFU));
        onRoutable();
    }
}

/**
 * @brief Marks the service as routable and initializes network services.
 *
 * Terminates hotspot operation when transitioning from hotspot mode, starts
 * mDNS service registration, and triggers SNTP synchronization.
 */
void ConnectivityService::onRoutable()
{
    if (!Connectivity.routable)
    {
        Connectivity.routable = true;
        if (WiFiClass::getMode() != wifi_mode_t::WIFI_MODE_STA)
        {
            JsonDocument doc{};
            doc["event"].set("connected");
            Device.transmit(doc.as<JsonObjectConst>(), Connectivity.name, false);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
            ESP_LOGD(name.data(), "terminating Wi-Fi hotspot");
            Connectivity.dns.reset();
            WiFiClass::mode(wifi_mode_t::WIFI_MODE_STA);
        }
        if (!Connectivity.mdns && MDNS.begin(HOSTNAME))
        {
            Connectivity.mdns = true;
            MDNS.setInstanceName(NAME);
#if EXTENSION_ALEXA
            AlexaExtension::onMdns();
#endif // EXTENSION_ALEXA
#if EXTENSION_RESTFUL || EXTENSION_WEBAPP
            MDNS.addService("http", "tcp", 80U);
#endif // EXTENSION_RESTFUL || EXTENSION_WEBAPP
#if EXTENSION_WEBSOCKET
            MDNS.addService("ws", "tcp", 80U);
#endif // EXTENSION_WEBSOCKET
        }
        timeval tv{};
        sntp_sync_time(&tv);
    }
}

/**
 * @brief Processes completed Wi-Fi scans and transmits the discovered networks.
 *
 * Each network includes its SSID, signal strength, and encryption status.
 */
void ConnectivityService::onScan(arduino_event_id_t event)
{
    const int16_t count{WiFi.scanComplete()};
    if (count > 0)
    {
        JsonDocument doc{};
        for (int16_t idx{0}; idx < count; ++idx)
        {
            doc["scan"][idx]["encrypted"].set(WiFi.encryptionType(idx) != wifi_auth_mode_t::WIFI_AUTH_OPEN);
            doc["scan"][idx]["rssi"].set(WiFi.RSSI(idx));
            doc["scan"][idx]["ssid"].set(WiFi.SSID(idx));
        }
        Device.transmit(doc.as<JsonObjectConst>(), Connectivity.name, false);
    }
}

/**
 * @brief Transmits the current Wi-Fi connectivity status and saved network names.
 */
void ConnectivityService::transmit()
{
    JsonDocument doc{};
    doc["host"].set(HOSTNAME ".local");
    doc["rssi"].set(WiFi.RSSI());
    {
        nvs_handle_t handle{};
        if (nvs_open(name.data(), nvs_open_mode_t::NVS_READONLY, &handle) == ESP_OK)
        {
            size_t length{0U};
            if (nvs_get_blob(handle, "Wi-Fi", nullptr, &length) == ESP_OK && length != 0U)
            {
                JsonDocument _saved{};
                std::vector<char> buffer(length);
                if (nvs_get_blob(handle, "Wi-Fi", buffer.data(), &length) == ESP_OK &&
                    deserializeJson(_saved, buffer.data(), length) == DeserializationError::Code::Ok)
                {
                    JsonArray saved{doc["saved"].to<JsonArray>()};
                    for (const JsonPairConst pair : _saved.as<JsonObjectConst>())
                    {
                        saved.add(pair.key());
                    }
                }
            }
            nvs_close(handle);
        }
    }
    doc["ssid"].set(WiFi.SSID());
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

/**
 * @brief Handles incoming Wi-Fi credentials and scan requests.
 *
 * @param payload Incoming payload containing optional Wi-Fi credentials or an action.
 * @param source Source identifier for the payload.
 */
void ConnectivityService::onReceive(JsonObjectConst payload, std::string_view source)
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
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
        ESP_LOGD(name.data(), "scanning for Wi-Fi networks...");
        WiFi.scanNetworks(true);
    }
}

#if EXTENSION_HOMEASSISTANT
/**
 * @brief Adds a Home Assistant diagnostic sensor for Wi-Fi signal strength.
 *
 * @param discovery Home Assistant discovery document to update.
 * @param topic Base topic used to read the signal-strength value.
 * @param unique Prefix used to construct the sensor's unique identifier.
 */
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
ConnectivityService &Connectivity{ConnectivityService::getInstance()};
