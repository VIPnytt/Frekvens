#include "modules/ExtensionModule.h"

void ExtensionModule::configure() {}
void ExtensionModule::begin() {}
void ExtensionModule::handle() {}

void ExtensionModule::onReceive(const JsonDocument &doc, const char *source) {}
void ExtensionModule::onTransmit(const JsonDocument &doc, const char *source) {}
