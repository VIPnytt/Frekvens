#include "modules/ExtensionModule.h"

void ExtensionModule::configure() {}
void ExtensionModule::begin() {}
void ExtensionModule::handle() {}

void ExtensionModule::onReceive(JsonObjectConst payload, std::string_view source) {}
void ExtensionModule::onTransmit(JsonObjectConst payload, std::string_view source) {}
