#include "modules/ExtensionModule.h"

void ExtensionModule::configure() {}
void ExtensionModule::begin() {}
void ExtensionModule::handle() {}

void ExtensionModule::onReceive(JsonObjectConst payload, std::string_view source) {}
/**
 * @brief Handles a payload transmitted by the extension.
 *
 * @param payload JSON payload being transmitted.
 * @param source Identifier of the payload's source.
 */
void ExtensionModule::onTransmit(JsonObjectConst payload, std::string_view source) {}

#if EXTENSION_HOMEASSISTANT
/**
 * @brief Handles Home Assistant discovery data.
 *
 * @param discovery Home Assistant discovery data.
 * @param topic Topic associated with the discovery data.
 * @param unique Unique identifier for the discovery entry.
 */
void ExtensionModule::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) {}
#endif // EXTENSION_HOMEASSISTANT
