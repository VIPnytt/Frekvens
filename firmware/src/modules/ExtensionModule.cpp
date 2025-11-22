#include "modules/ExtensionModule.h"

void ExtensionModule::setup() {}
void ExtensionModule::ready() {}
void ExtensionModule::handle() {}

void ExtensionModule::receiverHook(const JsonDocument doc, const char *const source) {}
void ExtensionModule::transmitterHook(const JsonDocument &doc, const char *const source) {}
