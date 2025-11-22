#include <HTTPClient.h>
#include <NetworkClientSecure.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <Preferences.h>
#include <regex>

#include "extensions/HomeAssistantExtension.h"
#include "extensions/MqttExtension.h"
#include "extensions/ServerSentEventsExtension.h"
#include "extensions/WebSocketExtension.h"
#include "fonts/LargeFont.h"
#include "handlers/TextHandler.h"
#include "services/ConnectivityService.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"
#include "services/FontsService.h"
#include "services/ModesService.h"
#include "services/WebServerService.h"

void DeviceService::init()
{
    Serial.begin(MONITOR_SPEED);
    vTaskDelay(UINT8_MAX);
    ESP_LOGI(name, "Frekvens " VERSION);
    ESP_LOGD(name, MANUFACTURER " " MODEL);

#if SOC_PM_SUPPORT_EXT_WAKEUP || SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP
#if SOC_GPIO_SUPPORT_HOLD_IO_IN_DSLP && !SOC_GPIO_SUPPORT_HOLD_SINGLE_IO_IN_DSLP
    gpio_deep_sleep_hold_dis();
#endif // SOC_GPIO_SUPPORT_HOLD_IO_IN_DSLP && !SOC_GPIO_SUPPORT_HOLD_SINGLE_IO_IN_DSLP
#ifdef PIN_INT
    gpio_pullup_en((gpio_num_t)PIN_INT);
    gpio_hold_en((gpio_num_t)PIN_INT);
#endif // PIN_INT
#ifdef PIN_SW1
    gpio_pullup_en((gpio_num_t)PIN_SW1);
    gpio_hold_en((gpio_num_t)PIN_SW1);
#endif // PIN_SW1
#ifdef PIN_SW2
    gpio_pullup_en((gpio_num_t)PIN_SW2);
    gpio_hold_en((gpio_num_t)PIN_SW2);
#endif // PIN_SW2
#if defined(PIN_INT) || defined(PIN_SW1) || defined(PIN_SW2)
#if SOC_GPIO_SUPPORT_HOLD_IO_IN_DSLP && !SOC_GPIO_SUPPORT_HOLD_SINGLE_IO_IN_DSLP
    gpio_deep_sleep_hold_en();
#endif // SOC_GPIO_SUPPORT_HOLD_IO_IN_DSLP && !SOC_GPIO_SUPPORT_HOLD_SINGLE_IO_IN_DSLP
#endif // defined(PIN_INT) || defined(PIN_SW1) || defined(PIN_SW2)
#endif // SOC_PM_SUPPORT_EXT_WAKEUP || SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP

#if SOC_PM_SUPPORT_EXT_WAKEUP && CONFIG_IDF_TARGET_ESP32 && defined(PIN_INT) && defined(PIN_SW1)
    esp_sleep_enable_ext0_wakeup((gpio_num_t)PIN_INT, LOW);
    esp_sleep_enable_ext1_wakeup(1ULL << PIN_SW1, ESP_EXT1_WAKEUP_ALL_LOW);
#elif SOC_PM_SUPPORT_EXT_WAKEUP && CONFIG_IDF_TARGET_ESP32 && defined(PIN_INT) && defined(PIN_SW2)
    esp_sleep_enable_ext0_wakeup((gpio_num_t)PIN_INT, LOW);
    esp_sleep_enable_ext1_wakeup(1ULL << PIN_SW2, ESP_EXT1_WAKEUP_ALL_LOW);
#elif SOC_PM_SUPPORT_EXT_WAKEUP && CONFIG_IDF_TARGET_ESP32 && defined(PIN_SW1) && defined(PIN_SW2)
    esp_sleep_enable_ext0_wakeup((gpio_num_t)PIN_SW1, LOW);
    esp_sleep_enable_ext1_wakeup(1ULL << PIN_SW2, ESP_EXT1_WAKEUP_ALL_LOW);
#elif SOC_PM_SUPPORT_EXT_WAKEUP && defined(PIN_INT) && defined(PIN_SW1) && defined(PIN_SW2)
    esp_sleep_enable_ext1_wakeup((1ULL << PIN_INT) | (1ULL << PIN_SW1) | (1ULL << PIN_SW2), ESP_EXT1_WAKEUP_ANY_LOW);
#elif SOC_PM_SUPPORT_EXT_WAKEUP && defined(PIN_INT) && defined(PIN_SW1)
    esp_sleep_enable_ext1_wakeup((1ULL << PIN_INT) | (1ULL << PIN_SW1), ESP_EXT1_WAKEUP_ANY_LOW);
#elif SOC_PM_SUPPORT_EXT_WAKEUP && defined(PIN_INT) && defined(PIN_SW2)
    esp_sleep_enable_ext1_wakeup((1ULL << PIN_INT) | (1ULL << PIN_SW2), ESP_EXT1_WAKEUP_ANY_LOW);
#elif SOC_PM_SUPPORT_EXT_WAKEUP && defined(PIN_SW1) && defined(PIN_SW2)
    esp_sleep_enable_ext1_wakeup((1ULL << PIN_SW1) | (1ULL << PIN_SW2), ESP_EXT1_WAKEUP_ANY_LOW);
#elif SOC_PM_SUPPORT_EXT_WAKEUP && defined(PIN_INT)
    esp_sleep_enable_ext0_wakeup((gpio_num_t)PIN_INT, LOW);
#elif SOC_PM_SUPPORT_EXT_WAKEUP && defined(PIN_SW1)
    esp_sleep_enable_ext0_wakeup((gpio_num_t)PIN_SW1, LOW);
#elif SOC_PM_SUPPORT_EXT_WAKEUP && defined(PIN_SW2)
    esp_sleep_enable_ext0_wakeup((gpio_num_t)PIN_SW2, LOW);
#elif SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP && defined(PIN_INT) && defined(PIN_SW1) && defined(PIN_SW2)
    esp_deep_sleep_enable_gpio_wakeup((1ULL << PIN_INT) | (1ULL << PIN_SW1) | (1ULL << PIN_SW2), ESP_GPIO_WAKEUP_GPIO_LOW);
#elif SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP && defined(PIN_INT) && defined(PIN_SW1)
    esp_deep_sleep_enable_gpio_wakeup((1ULL << PIN_INT) | (1ULL << PIN_SW1), ESP_GPIO_WAKEUP_GPIO_LOW);
#elif SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP && defined(PIN_INT) && defined(PIN_SW2)
    esp_deep_sleep_enable_gpio_wakeup((1ULL << PIN_INT) | (1ULL << PIN_SW2), ESP_GPIO_WAKEUP_GPIO_LOW);
#elif SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP && defined(PIN_SW1) && defined(PIN_SW2)
    esp_deep_sleep_enable_gpio_wakeup((1ULL << PIN_SW1) | (1ULL << PIN_SW2), ESP_GPIO_WAKEUP_GPIO_LOW);
#elif SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP && defined(PIN_INT)
    esp_deep_sleep_enable_gpio_wakeup(1ULL << PIN_INT, ESP_GPIO_WAKEUP_GPIO_LOW);
#elif SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP && defined(PIN_SW1)
    esp_deep_sleep_enable_gpio_wakeup(1ULL << PIN_SW1, ESP_GPIO_WAKEUP_GPIO_LOW);
#elif SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP && defined(PIN_SW2)
    esp_deep_sleep_enable_gpio_wakeup(1ULL << PIN_SW2, ESP_GPIO_WAKEUP_GPIO_LOW);
#endif // SOC_PM_SUPPORT_EXT_WAKEUP && CONFIG_IDF_TARGET_ESP32 && defined(PIN_INT) && defined(PIN_SW1)

    taskHandle = xTaskGetCurrentTaskHandle();

    Display.setup();
    Connectivity.setup();
    WebServer.setup();
    Extensions.setup();
    Modes.setup();
    ready();
}

void DeviceService::ready()
{
    operational = true;
    ESP_LOGV(name, "operational");

#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_reboot");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::command_template] = "{\"action\":\"{{value}}\"}";
        component[HomeAssistantAbbreviations::command_topic] = topic + "/set";
        component[HomeAssistantAbbreviations::device_class] = "restart";
        component[HomeAssistantAbbreviations::enabled_by_default] = false;
        component[HomeAssistantAbbreviations::entity_category] = "config";
        component[HomeAssistantAbbreviations::name] = "Reboot";
        component[HomeAssistantAbbreviations::object_id] = HOSTNAME "_" + id;
        component[HomeAssistantAbbreviations::payload_press] = "reboot";
        component[HomeAssistantAbbreviations::platform] = "button";
        component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
    }
    {
        const std::string id = std::string(name).append("_power");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::command_template] = "{\"action\":\"{{value}}\"}";
        component[HomeAssistantAbbreviations::command_topic] = topic + "/set";
        component[HomeAssistantAbbreviations::entity_category] = "config";
        component[HomeAssistantAbbreviations::icon] = "mdi:power";
        component[HomeAssistantAbbreviations::name] = "Power off";
        component[HomeAssistantAbbreviations::object_id] = HOSTNAME "_" + id;
        component[HomeAssistantAbbreviations::payload_press] = "power";
        component[HomeAssistantAbbreviations::platform] = "button";
        component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
    }
    {
        const std::string id = std::string(name).append("_temperature");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::device_class] = "temperature";
        component[HomeAssistantAbbreviations::enabled_by_default] = false;
        component[HomeAssistantAbbreviations::entity_category] = "diagnostic";
        component[HomeAssistantAbbreviations::expire_after] = UINT8_MAX;
        component[HomeAssistantAbbreviations::force_update] = true;
        component[HomeAssistantAbbreviations::name] = "Internal temperature";
        component[HomeAssistantAbbreviations::object_id] = HOSTNAME "_" + id;
        component[HomeAssistantAbbreviations::platform] = "sensor";
        component[HomeAssistantAbbreviations::state_class] = "measurement";
        component[HomeAssistantAbbreviations::state_topic] = topic;
        component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[HomeAssistantAbbreviations::unit_of_measurement] = "°C";
        component[HomeAssistantAbbreviations::value_template] = "{{value_json.temperature}}";
    }
#endif // EXTENSION_HOMEASSISTANT
    Display.ready();
    Connectivity.ready();
    Fonts.ready();
    Extensions.ready();
    Modes.ready();
    transmit();
    ESP_LOGD(name, "ready");
}

void DeviceService::run()
{
    Connectivity.handle();
    Display.handle();
    Modes.handle();
    if (millis() - lastMillis > UINT16_MAX)
    {
        transmit();
    }
}

void DeviceService::setPower(bool power, const char *const source)
{
    if (power)
    {
        ESP_LOGI(source, "rebooting...");
    }
    else
    {
        ESP_LOGW(source, "powering off...");
    }
    JsonDocument doc;
    doc["event"] = power ? "reboot" : "power";
    Device.transmit(doc, name, false);
    if (Modes.active && eTaskGetState(Modes.taskHandle) != eTaskState::eSuspended)
    {
        Modes.active->sleep();
        Modes.setActive(false);
    }
    Display.setPower(false, source);
    Display.clearFrame();
    Display.flush();
#if EXTENSION_MQTT
    Mqtt->disconnect();
#endif
#if EXTENSION_SERVERSENTEVENTS
    ServerSentEvents->client->close();
#endif
#if EXTENSION_WEBSOCKET
    WebSocket->server->closeAll();
#endif
    WiFi.disconnect(true);
    power ? ESP.restart() : esp_deep_sleep_start();
}

void DeviceService::restore()
{
    ESP_LOGW(name, "restoring...");
    if (Modes.active && eTaskGetState(Modes.taskHandle) != eTaskState::eSuspended)
    {
        Modes.setActive(false);
    }
    Display.setPower(false, name);
#if EXTENSION_HOMEASSISTANT
    HomeAssistant->undiscover();
#endif
#if EXTENSION_MQTT
    Mqtt->client.loop();
    Mqtt->client.disconnect();
#endif
#if EXTENSION_SERVERSENTEVENTS
    ServerSentEvents->client->close();
#endif
#if EXTENSION_WEBSOCKET
    WebSocket->server->closeAll();
#endif
    WiFi.disconnect(true, true);
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
#if SOC_GPIO_SUPPORT_HOLD_IO_IN_DSLP && !SOC_GPIO_SUPPORT_HOLD_SINGLE_IO_IN_DSLP
    gpio_deep_sleep_hold_dis();
#endif // SOC_GPIO_SUPPORT_HOLD_IO_IN_DSLP && !SOC_GPIO_SUPPORT_HOLD_SINGLE_IO_IN_DSLP
    nvs_flash_erase();
    esp_deep_sleep_start();
}

const std::vector<const char *> DeviceService::getNames() const
{
    std::vector<const char *> modules = {
        Device.name,
        Display.name,
        Extensions.name,
        Fonts.name,
        Modes.name,
        Connectivity.name,
        WebServer.name,
    };
    for (const ExtensionModule *extension : Extensions.getAll())
    {
        modules.push_back(extension->name);
    }
    for (const ModeModule *mode : Modes.getAll())
    {
        modules.push_back(mode->name);
    }
    return modules;
}

const JsonDocument DeviceService::getTransmits() const
{
    return transmits;
}

void DeviceService::transmit()
{
    JsonDocument doc;
    doc["board"] = BOARD__NAME;
    doc["model"] = MODEL;
    doc["name"] = NAME;
    doc["project"] = "https://github.com/VIPnytt/Frekvens";
    doc["temperature"] = temperatureRead();
    doc["version"] = VERSION;
    lastMillis = millis();
    Device.transmit(doc, name);
}

void DeviceService::transmit(JsonDocument doc, const char *const source, bool retain)
{
    if (retain)
    {
        transmits[source] = doc;
    }
    if (operational)
    {
        ESP_LOGV(source, "transmitting");
        for (ExtensionModule *extension : Extensions.getAll())
        {
            extension->transmitterHook(doc, source);
        }
    }
}

void DeviceService::receive(const JsonDocument doc, const char *const source, const char *const destination)
{
    if (operational)
    {
        ESP_LOGV(source, "receiving");
        ServiceModule *services[] = {
            &Connectivity,
            &Device,
            &Display,
            &Modes,
        };
        for (ServiceModule *service : services)
        {
            if (!strcmp(service->name, destination))
            {
                service->receiverHook(doc, source);
                return;
            }
        }
        for (ExtensionModule *extension : Extensions.getAll())
        {
            if (!strcmp(extension->name, destination))
            {
                extension->receiverHook(doc, source);
                return;
            }
        }
        for (ModeModule *mode : Modes.getAll())
        {
            if (!strcmp(mode->name, destination))
            {
                mode->receiverHook(doc, source);
                return;
            }
        }
    }
}

void DeviceService::receiverHook(const JsonDocument doc, const char *const source)
{
    if (doc["action"].is<const char *>())
    {
        const char *const action = doc["action"].as<const char *>();
        // Power off
        if (!strcmp(action, "power"))
        {
            setPower(false, source);
        }
        // Reboot
        else if (!strcmp(action, "reboot"))
        {
            setPower(true, source);
        }
        // Restore
        else if (!strcmp(action, "restore"))
        {
            restore();
        }
    }
}

DeviceService &DeviceService::getInstance()
{
    static DeviceService instance;
    return instance;
}

DeviceService &Device = Device.getInstance();
