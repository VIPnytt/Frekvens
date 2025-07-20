#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"
#include "services/ModesService.h"

void ExtensionsService::setup()
{
    for (ExtensionModule *extension : modules)
    {
        extension->setup();
    }
#ifdef F_VERBOSE
    Serial.print(name);
    Serial.println(": setup complete");
#endif
}

void ExtensionsService::ready()
{
#if EXTENSION_BUILD && defined(TASK_STACK_EXTENSIONS)
    (*Build->config)[Config::h][__STRING(TASK_STACK_EXTENSIONS)] = TASK_STACK_EXTENSIONS;
#endif
#if EXTENSION_BUILD && defined(EXTENSION_ALEXA) && !(EXTENSION_ALEXA)
    (*Build->config)[Config::h][__STRING(EXTENSION_ALEXA)] = false;
#endif
#if EXTENSION_BUILD && defined(EXTENSION_BUTTON) && !(EXTENSION_BUTTON)
    (*Build->config)[Config::h][__STRING(EXTENSION_BUTTON)] = false;
#endif
#if EXTENSION_BUILD && defined(EXTENSION_HOMEASSISTANT) && !(EXTENSION_HOMEASSISTANT)
    (*Build->config)[Config::h][__STRING(EXTENSION_HOMEASSISTANT)] = false;
#endif
#if EXTENSION_BUILD && defined(EXTENSION_INFRARED) && !(EXTENSION_INFRARED)
    (*Build->config)[Config::h][__STRING(EXTENSION_INFRARED)] = false;
#endif
#if EXTENSION_BUILD && defined(EXTENSION_MESSAGE) && !(EXTENSION_MESSAGE)
    (*Build->config)[Config::h][__STRING(EXTENSION_MESSAGE)] = false;
#endif
#if EXTENSION_BUILD && defined(EXTENSION_MICROPHONE) && !(EXTENSION_MICROPHONE)
    (*Build->config)[Config::h][__STRING(EXTENSION_MICROPHONE)] = false;
#endif
#if EXTENSION_BUILD && defined(EXTENSION_MQTT) && !(EXTENSION_MQTT)
    (*Build->config)[Config::h][__STRING(EXTENSION_MQTT)] = false;
#endif
#if EXTENSION_BUILD && defined(EXTENSION_OTA) && !(EXTENSION_OTA)
    (*Build->config)[Config::h][__STRING(EXTENSION_OTA)] = false;
#endif
#if EXTENSION_BUILD && defined(EXTENSION_PLAYLIST) && !(EXTENSION_PLAYLIST)
    (*Build->config)[Config::h][__STRING(EXTENSION_PLAYLIST)] = false;
#endif
#if EXTENSION_BUILD && defined(EXTENSION_RESTFUL) && !(EXTENSION_RESTFUL)
    (*Build->config)[Config::h][__STRING(EXTENSION_RESTFUL)] = false;
#endif
#if EXTENSION_BUILD && defined(EXTENSION_RTC) && !(EXTENSION_RTC)
    (*Build->config)[Config::h][__STRING(EXTENSION_RTC)] = false;
#endif
#if EXTENSION_BUILD && defined(EXTENSION_SIGNAL) && !(EXTENSION_SIGNAL)
    (*Build->config)[Config::h][__STRING(EXTENSION_SIGNAL)] = false;
#endif
#if EXTENSION_BUILD && defined(EXTENSION_WEBAPP) && !(EXTENSION_WEBAPP)
    (*Build->config)[Config::h][__STRING(EXTENSION_WEBAPP)] = false;
#endif
#if EXTENSION_BUILD && defined(EXTENSION_WEBSOCKET) && !(EXTENSION_WEBSOCKET)
    (*Build->config)[Config::h][__STRING(EXTENSION_WEBSOCKET)] = false;
#endif

#if EXTENSION_HOMEASSISTANT
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_stack");
        JsonObject component = (*HomeAssistant->discovery)[Abbreviations::components][id].to<JsonObject>();
        component[Abbreviations::device_class] = "data_size";
        component[Abbreviations::enabled_by_default] = false;
        component[Abbreviations::entity_category] = "diagnostic";
        component[Abbreviations::icon] = "mdi:memory";
        component[Abbreviations::name] = std::string(name).append(" stack");
        component[Abbreviations::object_id] = HOSTNAME "_" + id;
        component[Abbreviations::platform] = "sensor";
        component[Abbreviations::state_class] = "measurement";
        component[Abbreviations::state_topic] = topic;
        component[Abbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[Abbreviations::unit_of_measurement] = "kB";
        component[Abbreviations::value_template] = "{{value_json.stack/2**10 }}";
    }
#endif // EXTENSION_HOMEASSISTANT
    for (ExtensionModule *extension : modules)
    {
        extension->ready();
    }
    xTaskCreate(&onTask, name, stackSize, nullptr, 1, &taskHandle);
    transmit();
}

void ExtensionsService::handle()
{
    if (millis() - lastMillis > UINT16_MAX)
    {
        transmit();
    }
}

const std::vector<ExtensionModule *> &ExtensionsService::getAll() const
{
    return modules;
}

void ExtensionsService::transmit()
{
    JsonDocument doc;
    JsonArray list = doc["list"].to<JsonArray>();
    for (const ExtensionModule *extension : modules)
    {
        list.add(extension->name);
    }
    lastMillis = millis();
    doc["stack"] = stackSize - uxTaskGetStackHighWaterMark(taskHandle);
    Device.transmit(doc, name);
}

void ExtensionsService::onTask(void *parameter)
{
    for (;;)
    {
        for (ExtensionModule *extension : Extensions.getAll())
        {
            extension->handle();
        }
        vTaskDelay(1);
    }
}

ExtensionsService &ExtensionsService::getInstance()
{
    static ExtensionsService instance;
    return instance;
}

ExtensionsService &Extensions = Extensions.getInstance();
