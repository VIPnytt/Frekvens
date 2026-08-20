#include "modules/ModeModule.h"

void ModeModule::configure() {}
void ModeModule::begin() {}
void ModeModule::handle() {}
void ModeModule::end() {}

/**
 * @brief Handles a received payload from a source.
 *
 * @param payload Received JSON payload.
 * @param source Identifier of the payload source.
 */
void ModeModule::onReceive(JsonObjectConst payload, std::string_view source) {}

#if EXTENSION_HOMEASSISTANT
/**
 * @brief Handles Home Assistant discovery data for the mode module.
 *
 * @param discovery Home Assistant discovery document to configure.
 * @param topic MQTT topic associated with the discovery data.
 * @param unique Unique identifier for the Home Assistant entity.
 */
void ModeModule::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) {}
#endif // EXTENSION_HOMEASSISTANT
