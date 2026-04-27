#include "services/DeviceService.h"

#include "services/ConnectivityService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"
#include "services/FontsService.h"
#include "services/ModesService.h"
#include "services/WebServerService.h"

#include <array>
#include <nvs_flash.h>
#include <string_view>

void DeviceService::begin()
{
    Serial.begin(MONITOR_SPEED);
    vTaskDelay(INT8_MAX);
    ESP_LOGI("Device", "Frekvens " VERSION);    // NOLINT(cppcoreguidelines-avoid-do-while)
    ESP_LOGD("Device", MANUFACTURER " " MODEL); // NOLINT(cppcoreguidelines-avoid-do-while)
#if SOC_PM_SUPPORT_EXT_WAKEUP || SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP
#if SOC_GPIO_SUPPORT_HOLD_IO_IN_DSLP && !SOC_GPIO_SUPPORT_HOLD_SINGLE_IO_IN_DSLP
    gpio_deep_sleep_hold_dis();
#endif // SOC_GPIO_SUPPORT_HOLD_IO_IN_DSLP && !SOC_GPIO_SUPPORT_HOLD_SINGLE_IO_IN_DSLP
#ifdef PIN_INT
    gpio_pullup_en(static_cast<gpio_num_t>(PIN_INT));
    gpio_hold_en(static_cast<gpio_num_t>(PIN_INT));
#endif // PIN_INT
#ifdef PIN_SW1
    gpio_pullup_en(static_cast<gpio_num_t>(PIN_SW1));
    gpio_hold_en(static_cast<gpio_num_t>(PIN_SW1));
#endif // PIN_SW1
#ifdef PIN_SW2
    gpio_pullup_en(static_cast<gpio_num_t>(PIN_SW2));
    gpio_hold_en(static_cast<gpio_num_t>(PIN_SW2));
#endif // PIN_SW2
#if defined(PIN_INT) || defined(PIN_SW1) || defined(PIN_SW2)
#if SOC_GPIO_SUPPORT_HOLD_IO_IN_DSLP && !SOC_GPIO_SUPPORT_HOLD_SINGLE_IO_IN_DSLP
    gpio_deep_sleep_hold_en();
#endif // SOC_GPIO_SUPPORT_HOLD_IO_IN_DSLP && !SOC_GPIO_SUPPORT_HOLD_SINGLE_IO_IN_DSLP
#endif // defined(PIN_INT) || defined(PIN_SW1) || defined(PIN_SW2)
#endif // SOC_PM_SUPPORT_EXT_WAKEUP || SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP

#if SOC_PM_SUPPORT_EXT_WAKEUP && CONFIG_IDF_TARGET_ESP32 && defined(PIN_INT) && defined(PIN_SW1)
    esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(PIN_INT), LOW);
    esp_sleep_enable_ext1_wakeup(1ULL << static_cast<unsigned>(PIN_SW1), ESP_EXT1_WAKEUP_ALL_LOW);
#elif SOC_PM_SUPPORT_EXT_WAKEUP && CONFIG_IDF_TARGET_ESP32 && defined(PIN_INT) && defined(PIN_SW2)
    esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(PIN_INT), LOW);
    esp_sleep_enable_ext1_wakeup(1ULL << static_cast<unsigned>(PIN_SW2), ESP_EXT1_WAKEUP_ALL_LOW);
#elif SOC_PM_SUPPORT_EXT_WAKEUP && CONFIG_IDF_TARGET_ESP32 && defined(PIN_SW1) && defined(PIN_SW2)
    esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(PIN_SW1), LOW);
    esp_sleep_enable_ext1_wakeup(1ULL << static_cast<unsigned>(PIN_SW2), ESP_EXT1_WAKEUP_ALL_LOW);
#elif SOC_PM_SUPPORT_EXT_WAKEUP && defined(PIN_INT) && defined(PIN_SW1) && defined(PIN_SW2)
    esp_sleep_enable_ext1_wakeup((1ULL << static_cast<unsigned>(PIN_INT)) | (1ULL << static_cast<unsigned>(PIN_SW1)) |
                                     (1ULL << static_cast<unsigned>(PIN_SW2)),
                                 ESP_EXT1_WAKEUP_ANY_LOW);
#elif SOC_PM_SUPPORT_EXT_WAKEUP && defined(PIN_INT) && defined(PIN_SW1)
    esp_sleep_enable_ext1_wakeup((1ULL << static_cast<unsigned>(PIN_INT)) | (1ULL << static_cast<unsigned>(PIN_SW1)),
                                 ESP_EXT1_WAKEUP_ANY_LOW);
#elif SOC_PM_SUPPORT_EXT_WAKEUP && defined(PIN_INT) && defined(PIN_SW2)
    esp_sleep_enable_ext1_wakeup((1ULL << static_cast<unsigned>(PIN_INT)) | (1ULL << static_cast<unsigned>(PIN_SW2)),
                                 ESP_EXT1_WAKEUP_ANY_LOW);
#elif SOC_PM_SUPPORT_EXT_WAKEUP && defined(PIN_SW1) && defined(PIN_SW2)
    esp_sleep_enable_ext1_wakeup((1ULL << static_cast<unsigned>(PIN_SW1)) | (1ULL << static_cast<unsigned>(PIN_SW2)),
                                 ESP_EXT1_WAKEUP_ANY_LOW);
#elif SOC_PM_SUPPORT_EXT_WAKEUP && defined(PIN_INT)
    esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(PIN_INT), LOW);
#elif SOC_PM_SUPPORT_EXT_WAKEUP && defined(PIN_SW1)
    esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(PIN_SW1), LOW);
#elif SOC_PM_SUPPORT_EXT_WAKEUP && defined(PIN_SW2)
    esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(PIN_SW2), LOW);
#elif SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP && defined(PIN_INT) && defined(PIN_SW1) && defined(PIN_SW2)
    esp_deep_sleep_enable_gpio_wakeup((1ULL << static_cast<unsigned>(PIN_INT)) |
                                          (1ULL << static_cast<unsigned>(PIN_SW1)) |
                                          (1ULL << static_cast<unsigned>(PIN_SW2)),
                                      ESP_GPIO_WAKEUP_GPIO_LOW);
#elif SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP && defined(PIN_INT) && defined(PIN_SW1)
    esp_deep_sleep_enable_gpio_wakeup(
        (1ULL << static_cast<unsigned>(PIN_INT)) | (1ULL << static_cast<unsigned>(PIN_SW1)), ESP_GPIO_WAKEUP_GPIO_LOW);
#elif SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP && defined(PIN_INT) && defined(PIN_SW2)
    esp_deep_sleep_enable_gpio_wakeup(
        (1ULL << static_cast<unsigned>(PIN_INT)) | (1ULL << static_cast<unsigned>(PIN_SW2)), ESP_GPIO_WAKEUP_GPIO_LOW);
#elif SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP && defined(PIN_SW1) && defined(PIN_SW2)
    esp_deep_sleep_enable_gpio_wakeup(
        (1ULL << static_cast<unsigned>(PIN_SW1)) | (1ULL << static_cast<unsigned>(PIN_SW2)), ESP_GPIO_WAKEUP_GPIO_LOW);
#elif SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP && defined(PIN_INT)
    esp_deep_sleep_enable_gpio_wakeup(1ULL << static_cast<unsigned>(PIN_INT), ESP_GPIO_WAKEUP_GPIO_LOW);
#elif SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP && defined(PIN_SW1)
    esp_deep_sleep_enable_gpio_wakeup(1ULL << static_cast<unsigned>(PIN_SW1), ESP_GPIO_WAKEUP_GPIO_LOW);
#elif SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP && defined(PIN_SW2)
    esp_deep_sleep_enable_gpio_wakeup(1ULL << static_cast<unsigned>(PIN_SW2), ESP_GPIO_WAKEUP_GPIO_LOW);
#endif // SOC_PM_SUPPORT_EXT_WAKEUP && CONFIG_IDF_TARGET_ESP32 && defined(PIN_INT) && defined(PIN_SW1)
    taskHandle = xTaskGetCurrentTaskHandle();
    Display.configure();
    Connectivity.configure();
    WebServer.configure();
    Extensions.configure();
    Modes.configure();
    operational = true;
    ESP_LOGV("Status", "operational"); // NOLINT(cppcoreguidelines-avoid-do-while)
    WebServer.begin();
    Fonts.begin();
    Extensions.begin();
    Modes.begin();
    transmit();
    ESP_LOGD("Status", "ready"); // NOLINT(cppcoreguidelines-avoid-do-while)
}

void DeviceService::handle()
{
    Connectivity.handle();
    Display.handle();
    Modes.handle();
    if (millis() - lastMillis > UINT16_MAX)
    {
        transmit();
    }
}

void DeviceService::setPower(bool power)
{
    ESP_LOGI("Status", "%s...", power ? "rebooting" : "powering off"); // NOLINT(cppcoreguidelines-avoid-do-while)
    JsonDocument doc;                                                  // NOLINT(misc-const-correctness)
    doc["event"].set(power ? "reboot" : "power");
    Device.transmit(doc.as<JsonObjectConst>(), name, false);
    Modes.setActive(false);
    Display.setPower(false);
    Display.clearFrame();
    Display.flush();
#if EXTENSION_MQTT
    Extensions.MQTT().disconnect();
#endif
#if EXTENSION_SERVERSENTEVENTS
    Extensions.ServerSentEvents().events.close();
#endif
#if EXTENSION_WEBSOCKET
    Extensions.WebSocket().server->closeAll();
#endif
    WiFi.disconnect(true);
    power ? ESP.restart() : esp_deep_sleep_start();
}

void DeviceService::restore()
{
    ESP_LOGW("Status", "restoring..."); // NOLINT(cppcoreguidelines-avoid-do-while)
    Modes.setActive(false);
    Display.setPower(false);
#if EXTENSION_HOMEASSISTANT
    Extensions.HomeAssistant().undiscover();
#endif
#if EXTENSION_MQTT
    Extensions.MQTT().client.loop();
    Extensions.MQTT().client.disconnect();
#endif
#if EXTENSION_SERVERSENTEVENTS
    Extensions.ServerSentEvents().events.close();
#endif
#if EXTENSION_WEBSOCKET
    Extensions.WebSocket().server->closeAll();
#endif
    WiFi.disconnect(true, true);
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
#if SOC_GPIO_SUPPORT_HOLD_IO_IN_DSLP && !SOC_GPIO_SUPPORT_HOLD_SINGLE_IO_IN_DSLP
    gpio_deep_sleep_hold_dis();
#endif // SOC_GPIO_SUPPORT_HOLD_IO_IN_DSLP && !SOC_GPIO_SUPPORT_HOLD_SINGLE_IO_IN_DSLP
    nvs_flash_erase();
    esp_deep_sleep_start();
}

JsonObjectConst DeviceService::getTransmits() const { return transmits.as<JsonObjectConst>(); }

void DeviceService::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["board"].set(ARDUINO_BOARD);
    doc["model"].set(MODEL);
    doc["name"].set(NAME);
    doc["repository"].set("https://github.com/VIPnytt/Frekvens");
    doc["temperature"].set(temperatureRead());
    doc["version"].set(VERSION);
    lastMillis = millis();
    transmit(doc.as<JsonObjectConst>(), name);
}

// NOLINTNEXTLINE(readability-make-member-function-const)
void DeviceService::transmit(JsonObjectConst payload, std::string_view source, bool retain)
{
    if (retain)
    {
        transmits[source].set(payload);
    }
    if (operational)
    {
        ESP_LOGV("Status", "transmitting"); // NOLINT(cppcoreguidelines-avoid-do-while)
        for (ExtensionModule *extension : Extensions.getAll())
        {
            extension->onTransmit(payload, source);
        }
    }
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void DeviceService::receive(JsonObjectConst payload, std::string_view source, std::string_view destination) const
{
    if (operational)
    {
        ESP_LOGV("Status", "receiving"); // NOLINT(cppcoreguidelines-avoid-do-while)
        const std::array<ServiceModule *, 4> services{
            &Connectivity,
            &Device,
            &Display,
            &Modes,
        };
        for (ServiceModule *service : services)
        {
            if (service->name == destination)
            {
                service->onReceive(payload, source);
                return;
            }
        }
        for (ExtensionModule *extension : Extensions.getAll())
        {
            if (extension->name == destination)
            {
                extension->onReceive(payload, source);
                return;
            }
        }
        ModeModule *mode{Modes.getMode()};
        if (mode != nullptr && mode->name == destination)
        {
            mode->onReceive(payload, source);
            return;
        }
        std::unique_ptr<ModeModule> _mode{Modes.getMode(destination)};
        if (_mode != nullptr)
        {
            _mode->onReceive(payload, source);
        }
    }
}

void DeviceService::onReceive(JsonObjectConst payload,
                              std::string_view source) // NOLINT(misc-unused-parameters)
{
    if (payload["action"].is<const char *>())
    {
        const char *const action = payload["action"].as<const char *>(); // NOLINT(cppcoreguidelines-init-variables)
        // Power off
        if (strcmp(action, "power") == 0)
        {
            setPower(false);
        }
        // Reboot
        else if (strcmp(action, "reboot") == 0)
        {
            setPower(true);
        }
        // Restore
        else if (strcmp(action, "restore") == 0)
        {
            restore();
        }
    }
}

#if EXTENSION_HOMEASSISTANT
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void DeviceService::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique)
{
    topic.append(name);
    {
        const std::string id{std::string(name).append("_reboot")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"action":"{{value}}"})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::device_class].set("restart");
        component[HomeAssistantAbbreviations::enabled_by_default].set(false);
        component[HomeAssistantAbbreviations::entity_category].set("config");
        component[HomeAssistantAbbreviations::name].set("Reboot");
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        component[HomeAssistantAbbreviations::payload_press].set("reboot");
        component[HomeAssistantAbbreviations::platform].set("button");
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
    }
    {
        const std::string id{std::string(name).append("_power")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::command_template].set(R"({"action":"{{value}}"})");
        component[HomeAssistantAbbreviations::command_topic].set(topic + "/set");
        component[HomeAssistantAbbreviations::entity_category].set("config");
        component[HomeAssistantAbbreviations::icon].set("mdi:power");
        component[HomeAssistantAbbreviations::name].set("Power off");
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        component[HomeAssistantAbbreviations::payload_press].set("power");
        component[HomeAssistantAbbreviations::platform].set("button");
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
    }
    {
        const std::string id{std::string(name).append("_temperature")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::device_class].set("temperature");
        component[HomeAssistantAbbreviations::enabled_by_default].set(false);
        component[HomeAssistantAbbreviations::entity_category].set("diagnostic");
        component[HomeAssistantAbbreviations::expire_after].set(UINT8_MAX);
        component[HomeAssistantAbbreviations::force_update].set(true);
        component[HomeAssistantAbbreviations::name].set("Internal temperature");
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        component[HomeAssistantAbbreviations::platform].set("sensor");
        component[HomeAssistantAbbreviations::state_class].set("measurement");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::unit_of_measurement].set("°C");
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.temperature}}");
    }
}
#endif // EXTENSION_HOMEASSISTANT

DeviceService &DeviceService::getInstance()
{
    static DeviceService instance;
    return instance;
}

// NOLINTNEXTLINE(bugprone-throwing-static-initialization,cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables)
DeviceService &Device = DeviceService::getInstance();
