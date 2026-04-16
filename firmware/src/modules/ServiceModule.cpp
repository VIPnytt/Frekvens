#include "modules/ServiceModule.h"

void ServiceModule::onReceive(JsonObjectConst payload, std::string_view source) {}

#if EXTENSION_HOMEASSISTANT
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void ServiceModule::onHomeAssistant(JsonDocument &discovery, std::string topic, std::string unique) {}
#endif
