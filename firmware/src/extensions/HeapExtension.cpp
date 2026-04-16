#if EXTENSION_HEAP

#include "extensions/HeapExtension.h"

#include "services/DeviceService.h"
#include "services/ExtensionsService.h" // NOLINT(misc-include-cleaner)
#include "services/ModesService.h"

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
    JsonDocument doc; // NOLINT(misc-const-correctness)
    doc["extensions"].set(Extensions.stackSize - uxTaskGetStackHighWaterMark(Extensions.taskHandle));
    doc["heap"].set(ESP.getHeapSize() - ESP.getFreeHeap());
    doc["main"].set(CONFIG_ARDUINO_LOOP_STACK_SIZE - uxTaskGetStackHighWaterMark(Device.taskHandle));
    if (Modes.getTaskHandle() != nullptr)
    {
        doc["modes"].set(Modes.stackSize - uxTaskGetStackHighWaterMark(Modes.getTaskHandle()));
    }
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

#if EXTENSION_HOMEASSISTANT
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void HeapExtension::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique)
{
    topic.append(name);
    {
        const std::string id{std::string(name).append("_extensions")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::device_class].set("data_size");
        component[HomeAssistantAbbreviations::enabled_by_default].set(false);
        component[HomeAssistantAbbreviations::entity_category].set("diagnostic");
        component[HomeAssistantAbbreviations::expire_after].set(UINT8_MAX);
        component[HomeAssistantAbbreviations::icon].set("mdi:memory");
        component[HomeAssistantAbbreviations::name].set(std::string(Extensions.name.data()).append(" task stack"));
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        component[HomeAssistantAbbreviations::platform].set("sensor");
        component[HomeAssistantAbbreviations::state_class].set("measurement");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::unit_of_measurement].set("kB");
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.extensions/2**10}}");
    }
    {
        const std::string id{std::string(name).append("_heap")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::device_class].set("data_size");
        component[HomeAssistantAbbreviations::entity_category].set("diagnostic");
        component[HomeAssistantAbbreviations::expire_after].set(UINT8_MAX);
        component[HomeAssistantAbbreviations::icon].set("mdi:memory");
        component[HomeAssistantAbbreviations::name].set(name);
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        component[HomeAssistantAbbreviations::platform].set("sensor");
        component[HomeAssistantAbbreviations::state_class].set("measurement");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::unit_of_measurement].set("kB");
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.heap/2**10}}");
    }
    {
        const std::string id{std::string(name).append("_main")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::device_class].set("data_size");
        component[HomeAssistantAbbreviations::enabled_by_default].set(false);
        component[HomeAssistantAbbreviations::entity_category].set("diagnostic");
        component[HomeAssistantAbbreviations::expire_after].set(UINT8_MAX);
        component[HomeAssistantAbbreviations::icon].set("mdi:memory");
        component[HomeAssistantAbbreviations::name].set("Main task stack");
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        component[HomeAssistantAbbreviations::platform].set("sensor");
        component[HomeAssistantAbbreviations::state_class].set("measurement");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::unit_of_measurement].set("kB");
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.main/2**10}}");
    }
    {
        const std::string id{std::string(name).append("_modes")};
        JsonObject component{discovery[HomeAssistantAbbreviations::components][id].to<JsonObject>()};
        component[HomeAssistantAbbreviations::device_class].set("data_size");
        component[HomeAssistantAbbreviations::enabled_by_default].set(false);
        component[HomeAssistantAbbreviations::entity_category].set("diagnostic");
        component[HomeAssistantAbbreviations::expire_after].set(UINT8_MAX);
        component[HomeAssistantAbbreviations::icon].set("mdi:memory");
        component[HomeAssistantAbbreviations::name].set(std::string(Modes.name).append(" task stack"));
        component[HomeAssistantAbbreviations::object_id].set(HOSTNAME "_" + id);
        component[HomeAssistantAbbreviations::platform].set("sensor");
        component[HomeAssistantAbbreviations::state_class].set("measurement");
        component[HomeAssistantAbbreviations::state_topic].set(topic);
        component[HomeAssistantAbbreviations::unique_id].set(unique + id);
        component[HomeAssistantAbbreviations::unit_of_measurement].set("kB");
        component[HomeAssistantAbbreviations::value_template].set("{{value_json.modes/2**10}}");
    }
}
#endif // EXTENSION_HOMEASSISTANT

#endif // EXTENSION_HEAP
