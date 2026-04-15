#include "modules/ExtensionModule.h"

void ExtensionModule::configure() {}
void ExtensionModule::begin() {}
void ExtensionModule::handle() {}

void ExtensionModule::onReceive(JsonObjectConst payload, std::string_view source) {}
void ExtensionModule::onTransmit(JsonObjectConst payload, std::string_view source) {}

#if EXTENSION_HOMEASSISTANT
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void ExtensionModule::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) {}
#endif
