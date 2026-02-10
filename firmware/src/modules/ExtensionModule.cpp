#include "modules/ExtensionModule.h"

void ExtensionModule::configure() {}
void ExtensionModule::begin() {}
void ExtensionModule::handle() {}

void ExtensionModule::onReceive(JsonObjectConst payload, const char *source) {}
void ExtensionModule::onTransmit(JsonObjectConst payload, const char *source) {}
