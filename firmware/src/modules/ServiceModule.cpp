#include "modules/ServiceModule.h"

void ServiceModule::onReceive(JsonObjectConst payload, std::string_view source) {}

#if EXTENSION_HOMEASSISTANT
/**
 * @brief Handles Home Assistant discovery data.
 *
 * @param discovery Home Assistant discovery configuration.
 * @param topic MQTT topic associated with the discovery data.
 * @param unique Unique identifier for the Home Assistant entity.
 */
void ServiceModule::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) {}
#endif // EXTENSION_HOMEASSISTANT
