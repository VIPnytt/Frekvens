#if EXTENSION_HEAP

#include "extensions/HeapExtension.h"

#include "config/constants.h"
#include "extensions/HomeAssistantExtension.h"
#include "services/DeviceService.h"
#include "services/ExtensionsService.h"
#include "services/ModesService.h"

HeapExtension *Heap = nullptr;

HeapExtension::HeapExtension() : ExtensionModule("Heap") { Heap = this; }

#if EXTENSION_HOMEASSISTANT
void HeapExtension::configure()
{
    const std::string topic = std::string("frekvens/" HOSTNAME "/").append(name);
    {
        const std::string id = std::string(name).append("_extensions");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::device_class] = "data_size";
        component[HomeAssistantAbbreviations::enabled_by_default] = false;
        component[HomeAssistantAbbreviations::entity_category] = "diagnostic";
        component[HomeAssistantAbbreviations::expire_after] = UINT8_MAX;
        component[HomeAssistantAbbreviations::icon] = "mdi:memory";
        component[HomeAssistantAbbreviations::name] = std::string(Extensions.name).append(" task stack");
        component[HomeAssistantAbbreviations::object_id] = HOSTNAME "_" + id;
        component[HomeAssistantAbbreviations::platform] = "sensor";
        component[HomeAssistantAbbreviations::state_class] = "measurement";
        component[HomeAssistantAbbreviations::state_topic] = topic;
        component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[HomeAssistantAbbreviations::unit_of_measurement] = "kB";
        component[HomeAssistantAbbreviations::value_template] = "{{value_json.extensions/2**10}}";
    }
    {
        const std::string id = std::string(name).append("_heap");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::device_class] = "data_size";
        component[HomeAssistantAbbreviations::entity_category] = "diagnostic";
        component[HomeAssistantAbbreviations::expire_after] = UINT8_MAX;
        component[HomeAssistantAbbreviations::icon] = "mdi:memory";
        component[HomeAssistantAbbreviations::name] = name;
        component[HomeAssistantAbbreviations::object_id] = HOSTNAME "_" + id;
        component[HomeAssistantAbbreviations::platform] = "sensor";
        component[HomeAssistantAbbreviations::state_class] = "measurement";
        component[HomeAssistantAbbreviations::state_topic] = topic;
        component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[HomeAssistantAbbreviations::unit_of_measurement] = "kB";
        component[HomeAssistantAbbreviations::value_template] = "{{value_json.heap/2**10}}";
    }
    {
        const std::string id = std::string(name).append("_main");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::device_class] = "data_size";
        component[HomeAssistantAbbreviations::enabled_by_default] = false;
        component[HomeAssistantAbbreviations::entity_category] = "diagnostic";
        component[HomeAssistantAbbreviations::expire_after] = UINT8_MAX;
        component[HomeAssistantAbbreviations::icon] = "mdi:memory";
        component[HomeAssistantAbbreviations::name] = "Main task stack";
        component[HomeAssistantAbbreviations::object_id] = HOSTNAME "_" + id;
        component[HomeAssistantAbbreviations::platform] = "sensor";
        component[HomeAssistantAbbreviations::state_class] = "measurement";
        component[HomeAssistantAbbreviations::state_topic] = topic;
        component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[HomeAssistantAbbreviations::unit_of_measurement] = "kB";
        component[HomeAssistantAbbreviations::value_template] = "{{value_json.main/2**10}}";
    }
    {
        const std::string id = std::string(name).append("_modes");
        JsonObject component = (*HomeAssistant->discovery)[HomeAssistantAbbreviations::components][id].to<JsonObject>();
        component[HomeAssistantAbbreviations::device_class] = "data_size";
        component[HomeAssistantAbbreviations::enabled_by_default] = false;
        component[HomeAssistantAbbreviations::entity_category] = "diagnostic";
        component[HomeAssistantAbbreviations::expire_after] = UINT8_MAX;
        component[HomeAssistantAbbreviations::icon] = "mdi:memory";
        component[HomeAssistantAbbreviations::name] = std::string(Modes.name).append(" task stack");
        component[HomeAssistantAbbreviations::object_id] = HOSTNAME "_" + id;
        component[HomeAssistantAbbreviations::platform] = "sensor";
        component[HomeAssistantAbbreviations::state_class] = "measurement";
        component[HomeAssistantAbbreviations::state_topic] = topic;
        component[HomeAssistantAbbreviations::unique_id] = HomeAssistant->uniquePrefix + id;
        component[HomeAssistantAbbreviations::unit_of_measurement] = "kB";
        component[HomeAssistantAbbreviations::value_template] = "{{value_json.modes/2**10}}";
    }
}
#endif // EXTENSION_HOMEASSISTANT

void HeapExtension::handle()
{
    if (millis() - lastMillis > UINT16_MAX)
    {
        transmit();
        lastMillis = millis();
    }
}

void HeapExtension::transmit()
{
    JsonDocument doc;
    doc["extensions"] = Extensions.stackSize - uxTaskGetStackHighWaterMark(Extensions.taskHandle);
    doc["heap"] = ESP.getHeapSize() - ESP.getFreeHeap();
    doc["main"] = CONFIG_ARDUINO_LOOP_STACK_SIZE - uxTaskGetStackHighWaterMark(Device.taskHandle);
    if (Modes.getTaskHandle() != nullptr)
    {
        doc["modes"] = Modes.stackSize - uxTaskGetStackHighWaterMark(Modes.getTaskHandle());
    }
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

#endif // EXTENSION_HEAP
