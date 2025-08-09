#include <HTTPClient.h>
#include <nvs.h>
#include <Preferences.h>
#include <regex>
#include <vector>

#include "config/constants.h"
#include "extensions/BuildExtension.h"
#include "extensions/HomeAssistantExtension.h"
#include "extensions/MqttExtension.h"
#include "extensions/WebSocketExtension.h"
#include "fonts/LargeFont.h"
#include "handlers/TextHandler.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"
#include "services/FontsService.h"
#include "services/ModesService.h"
#include "services/NetworkService.h"

void DeviceService::init()
{
#ifdef MONITOR_SPEED
    Serial.begin(MONITOR_SPEED);
#endif
#ifdef F_VERBOSE
    delay(1 << 10);
#endif
#ifdef F_INFO
    Serial.printf("%s: Frekvens " VERSION "\n", name);
    Serial.printf("%s: " MANUFACTURER " " MODEL "\n", name);
    Serial.printf("%s: " BOARD_NAME "\n", name);
#endif

#if SOC_PM_SUPPORT_EXT_WAKEUP || SOC_GPIO_SUPPORT_DEEPSLEEP_WAKEUP
    gpio_deep_sleep_hold_dis();
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
    gpio_deep_sleep_hold_en();
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

    Display.setup();
    Network.setup();
    WebServer.setup();
    Extensions.setup();
    Modes.setup();
    ready();
}

void DeviceService::ready()
{
#if EXTENSION_BUILD
    (*Build->config)[Config::env][ENV] = "";
    (*Build->config)[Config::env][__STRING(NAME)] = NAME;
    (*Build->config)[Config::h][__STRING(PIN_CS)] = PIN_CS;
#ifdef PIN_CS2
    (*Build->config)[Config::h][__STRING(PIN_CS2)] = PIN_CS2;
#endif
    (*Build->config)[Config::h][__STRING(PIN_EN)] = PIN_EN;
#ifdef PIN_INT
    (*Build->config)[Config::h][__STRING(PIN_INT)] = PIN_INT;
#endif
#ifdef PIN_IR
    (*Build->config)[Config::h][__STRING(PIN_IR)] = PIN_IR;
#endif
#ifdef PIN_MIC
    (*Build->config)[Config::h][__STRING(PIN_MIC)] = PIN_MIC;
#endif
#ifdef PIN_MISO
    (*Build->config)[Config::h][__STRING(PIN_MISO)] = PIN_MISO;
#endif
#ifdef PIN_MISO2
    (*Build->config)[Config::h][__STRING(PIN_MISO2)] = PIN_MISO2;
#endif
    (*Build->config)[Config::h][__STRING(PIN_MOSI)] = PIN_MOSI;
#ifdef PIN_MOSI2
    (*Build->config)[Config::h][__STRING(PIN_MOSI2)] = PIN_MOSI2;
#endif
#ifdef PIN_SCL
    (*Build->config)[Config::h][__STRING(PIN_SCL)] = PIN_SCL;
#endif
    (*Build->config)[Config::h][__STRING(PIN_SCLK)] = PIN_SCLK;
#ifdef PIN_SCLK2
    (*Build->config)[Config::h][__STRING(PIN_SCLK2)] = PIN_SCLK2;
#endif
#ifdef PIN_SDA
    (*Build->config)[Config::h][__STRING(PIN_SDA)] = PIN_SDA;
#endif
#ifdef PIN_SDIO2
    (*Build->config)[Config::h][__STRING(PIN_SDIO2)] = PIN_SDIO2;
#endif
#ifdef PIN_SW1
    (*Build->config)[Config::h][__STRING(PIN_SW1)] = PIN_SW1;
#endif
#ifdef PIN_SW2
    (*Build->config)[Config::h][__STRING(PIN_SW2)] = PIN_SW2;
#endif
#ifdef LOCATION
    (*Build->config)[Config::h][__STRING(LOCATION)] = LOCATION;
#endif
#if defined(LATITUDE) && defined(LONGITUDE)
    (*Build->config)[Config::h][__STRING(LATITUDE)] = LATITUDE;
    (*Build->config)[Config::h][__STRING(LONGITUDE)] = LONGITUDE;
#endif
#ifdef BOARD
    (*Build->config)[Config::pio]["board"] = BOARD;
#endif
#ifdef BOARD_BUILD__F_CPU
    (*Build->config)[Config::pio]["board_build.f_cpu"] = BOARD_BUILD__F_CPU;
#endif
#ifdef BOARD_BUILD__F_FLASH
    (*Build->config)[Config::pio]["board_build.f_flash"] = BOARD_BUILD__F_FLASH;
#endif
#ifdef BOARD_BUILD__FLASH_MODE
    (*Build->config)[Config::pio]["board_build.flash_mode"] = BOARD_BUILD__FLASH_MODE;
#endif
#ifdef BOARD_BUILD__MCU
    (*Build->config)[Config::pio]["board_build.mcu"] = BOARD_BUILD__MCU;
#endif
#ifdef BOARD_BUILD__PARTITIONS
    (*Build->config)[Config::pio]["board_build.partitions"] = BOARD_BUILD__PARTITIONS;
#endif
#ifdef BOARD_UPLOAD__FLASH_SIZE
    (*Build->config)[Config::pio]["board_upload.flash_size"] = BOARD_UPLOAD__FLASH_SIZE;
#endif
#ifdef BOARD_UPLOAD__MAXIMUM_RAM_SIZE
    (*Build->config)[Config::pio]["board_upload.maximum_ram_size"] = BOARD_UPLOAD__MAXIMUM_RAM_SIZE;
#endif
#ifdef BOARD_UPLOAD__MAXIMUM_SIZE
    (*Build->config)[Config::pio]["board_upload.maximum_size"] = BOARD_UPLOAD__MAXIMUM_SIZE;
#endif
#endif // EXTENSION_BUILD
#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
#ifdef F_VERBOSE
    {
        const std::string id = std::string(name).append("_heap");
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::device_class] = "data_size";
        component[Abbreviations::enabled_by_default] = false;
        component[Abbreviations::entity_category] = "diagnostic";
        component[Abbreviations::expire_after] = UINT8_MAX;
        component[Abbreviations::force_update] = true;
        component[Abbreviations::icon] = "mdi:memory";
        component[Abbreviations::name] = "Heap";
        component[Abbreviations::object_id] = HOSTNAME "_" + id;
        component[Abbreviations::platform] = "sensor";
        component[Abbreviations::state_class] = "measurement";
        component[Abbreviations::state_topic] = topic;
        component[Abbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[Abbreviations::unit_of_measurement] = "kB";
        component[Abbreviations::value_template] = "{{value_json.heap/2**10}}";
    }
#endif // F_VERBOSE
    {
        const std::string id = std::string(name).append("_identify");
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::command_template] = "{\"action\":\"{{value}}\"}";
        component[Abbreviations::command_topic] = topic + "/set";
        component[Abbreviations::device_class] = "identify";
        component[Abbreviations::enabled_by_default] = false;
        component[Abbreviations::entity_category] = "config";
        component[Abbreviations::name] = "Identify";
        component[Abbreviations::object_id] = HOSTNAME "_" + id;
        component[Abbreviations::payload_press] = "identify";
        component[Abbreviations::platform] = "button";
        component[Abbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
    }
    {
        const std::string id = std::string(name).append("_reboot");
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::command_template] = "{\"action\":\"{{value}}\"}";
        component[Abbreviations::command_topic] = topic + "/set";
        component[Abbreviations::device_class] = "restart";
        component[Abbreviations::enabled_by_default] = false;
        component[Abbreviations::entity_category] = "config";
        component[Abbreviations::name] = "Reboot";
        component[Abbreviations::object_id] = HOSTNAME "_" + id;
        component[Abbreviations::payload_press] = "reboot";
        component[Abbreviations::platform] = "button";
        component[Abbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
    }
    {
        const std::string id = std::string(name).append("_power");
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::command_template] = "{\"action\":\"{{value}}\"}";
        component[Abbreviations::command_topic] = topic + "/set";
        component[Abbreviations::entity_category] = "config";
        component[Abbreviations::icon] = "mdi:power";
        component[Abbreviations::name] = "Power off";
        component[Abbreviations::object_id] = HOSTNAME "_" + id;
        component[Abbreviations::payload_press] = "power";
        component[Abbreviations::platform] = "button";
        component[Abbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
    }
#ifdef F_VERBOSE
    {
        const std::string id = std::string(name).append("_stack");
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::device_class] = "data_size";
        component[Abbreviations::enabled_by_default] = false;
        component[Abbreviations::entity_category] = "diagnostic";
        component[Abbreviations::icon] = "mdi:memory";
        component[Abbreviations::name] = "Main stack";
        component[Abbreviations::object_id] = HOSTNAME "_" + id;
        component[Abbreviations::platform] = "sensor";
        component[Abbreviations::state_class] = "measurement";
        component[Abbreviations::state_topic] = topic;
        component[Abbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[Abbreviations::unit_of_measurement] = "kB";
        component[Abbreviations::value_template] = "{{value_json.stack/2**10}}";
    }
#endif // F_VERBOSE
    {
        const std::string id = std::string(name).append("_temperature");
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::device_class] = "temperature";
        component[Abbreviations::enabled_by_default] = false;
        component[Abbreviations::entity_category] = "diagnostic";
        component[Abbreviations::expire_after] = UINT8_MAX;
        component[Abbreviations::force_update] = true;
        component[Abbreviations::name] = "Internal temperature";
        component[Abbreviations::object_id] = HOSTNAME "_" + id;
        component[Abbreviations::platform] = "sensor";
        component[Abbreviations::state_class] = "measurement";
        component[Abbreviations::state_topic] = topic;
        component[Abbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[Abbreviations::unit_of_measurement] = "°C";
        component[Abbreviations::value_template] = "{{value_json.temperature}}";
    }
    {
        const std::string id = std::string(name).append("_update");
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::device_class] = "firmware";
        component[Abbreviations::entity_category] = "diagnostic";
        component[Abbreviations::icon] = "mdi:github";
        component[Abbreviations::latest_version_template] = "{{value_json.version_available}}";
        component[Abbreviations::latest_version_topic] = topic;
        component[Abbreviations::name] = "Firmware";
        component[Abbreviations::object_id] = HOSTNAME "_" + id;
        component[Abbreviations::platform] = "update";
        component[Abbreviations::release_url] = std::string(repository).append("/releases/latest");
        component[Abbreviations::state_topic] = topic;
        component[Abbreviations::title] = "Frekvens";
        component[Abbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[Abbreviations::value_template] = "{{value_json.version_current}}";
    }
#endif // EXTENSION_HOMEASSISTANT
#if EXTENSION_BUILD
    Display.ready();
#endif // EXTENSION_BUILD
    Network.ready();
    Fonts.ready();
    Extensions.ready();
    Modes.ready();
    transmit();

#ifdef F_DEBUG
    Serial.printf("%s: ready\n", name);
#endif
}

void DeviceService::run()
{
    Network.handle();
    Display.handle();
#ifdef F_VERBOSE
    Extensions.handle();
#endif
    Modes.handle();
    if (millis() - lastMillis > UINT16_MAX)
    {
        if (latest.empty() && WiFi.isConnected())
        {
            update();
        }
        transmit();
    }
}

void DeviceService::update()
{
    HTTPClient http;
    http.begin("https://api.github.com/repos/VIPnytt/Frekvens/releases/latest");
    http.addHeader("Accept", "application/vnd.github+json");
    http.setUserAgent(Network.userAgent.data());
    if (http.GET() == t_http_codes::HTTP_CODE_OK)
    {
        JsonDocument doc;
        if (!deserializeJson(doc, http.getString()) && doc["tag_name"].is<std::string>())
        {
            latest = std::regex_replace(doc["tag_name"].as<std::string>(), std::regex(R"(^v)"), "");
            if (latest != VERSION)
            {
#ifdef F_INFO
                Serial.printf("%s: update available, " VERSION " -> %s - release notes @ %s/releases/latest\n", name, latest.c_str(), repository.data());
#endif
            }
        }
    }
}

void DeviceService::identify()
{
    Modes.set(false, name);
#ifdef F_VERBOSE
    Serial.printf("%s: identify\n", name);
#endif
    Display.clear();
    TextHandler("!", FontLarge).draw();
    Display.setGlobalBrightness(UINT8_MAX);
}

void DeviceService::power(bool state)
{
#ifdef F_INFO
    Serial.printf(state ? "%s: rebooting...\n" : "%s: powering off...\n", name);
#endif
    JsonDocument doc;
    doc["event"] = state ? "reboot" : "power";
    Device.transmit(doc, name, false);
    if (Modes.active && eTaskGetState(Modes.taskHandle) != eTaskState::eSuspended)
    {
        Modes.active->sleep();
        Modes.set(false, name);
    }
#if EXTENSION_MQTT
    Mqtt->disconnect();
#endif
#if EXTENSION_WEBSOCKET
    WebSocket->ws->closeAll();
#endif
    WiFi.disconnect(true);
    state ? ESP.restart() : esp_deep_sleep_start();
}

const std::vector<const char *> DeviceService::getNames() const
{
    std::vector<const char *> modules = {
        Device.name,
        Display.name,
        Extensions.name,
        Fonts.name,
        Modes.name,
        Network.name,
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
#ifdef BOARD_NAME
    doc["board"] = BOARD_NAME;
#endif
#ifdef F_VERBOSE
    doc["heap"] = ESP.getHeapSize() - ESP.getFreeHeap();
#endif
    doc["model"] = MODEL;
    doc["name"] = NAME;
    doc["releases_url"] = "https://github.com/VIPnytt/Frekvens/releases";
#ifdef F_VERBOSE
    doc["stack"] = CONFIG_ARDUINO_LOOP_STACK_SIZE - uxTaskGetStackHighWaterMark(nullptr);
#endif
    doc["temperature"] = temperatureRead();
    doc["version_current"] = VERSION;
    doc["version_available"] = latest.empty() ? VERSION : latest;
    lastMillis = millis();
    Device.transmit(doc, name);
}

void DeviceService::transmit(JsonDocument doc, const char *const source, bool retain)
{
#ifdef F_DEBUG
    tm local;
    if (doc["_timestamp"].isUnbound() && getLocalTime(&local))
    {
        char ts[30];
        strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", &local);
        doc["_timestamp"] = ts;
    }
#endif // F_DEBUG
#ifdef F_VERBOSE
    Serial.printf("%s: transmitting, %s\n", name, source);
#endif // F_VERBOSE
    if (retain)
    {
        transmits[source] = doc;
    }
    for (ExtensionModule *extension : Extensions.getAll())
    {
        extension->transmitterHook(doc, source);
    }
}

void DeviceService::receive(const JsonDocument doc, const char *const source, const char *const destination)
{
#ifdef F_VERBOSE
    Serial.printf("%s: receiving @ %s\n", name, source);
#endif
    ServiceModule *services[] = {
        &Device,
        &Display,
        &Modes,
        &Network,
    };
    for (ServiceModule *service : services)
    {
        if (!strcmp(service->name, destination))
        {
            service->receiverHook(doc);
            return;
        }
    }
    for (ExtensionModule *extension : Extensions.getAll())
    {
        if (!strcmp(extension->name, destination))
        {
            extension->receiverHook(doc);
            return;
        }
    }
    for (ModeModule *mode : Modes.getAll())
    {
        if (!strcmp(mode->name, destination))
        {
            mode->receiverHook(doc);
            return;
        }
    }
}

void DeviceService::receiverHook(const JsonDocument doc)
{
    if (doc["action"].is<const char *>())
    {
        const char *const action = doc["action"].as<const char *>();
        // Identify
        if (!strcmp(action, "identify"))
        {
            identify();
        }
        // Power off
        else if (!strcmp(action, "power"))
        {
            power(false);
        }
        // Reboot
        else if (!strcmp(action, "reboot"))
        {
            power(true);
        }
    }
}

DeviceService &DeviceService::getInstance()
{
    static DeviceService instance;
    return instance;
}

DeviceService &Device = Device.getInstance();
