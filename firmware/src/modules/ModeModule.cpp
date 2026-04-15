#include "modules/ModeModule.h"

void ModeModule::configure() {}
void ModeModule::begin() {}
void ModeModule::handle() {}
void ModeModule::end() {}

void ModeModule::onReceive(JsonObjectConst payload, std::string_view source) {}

#if EXTENSION_HOMEASSISTANT
void ModeModule::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) {}
#endif
