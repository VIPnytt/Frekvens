#include "config/constants.h"

#if EXTENSION_BUILD

#include <ArduinoJson.h>

#include "extensions/BuildExtension.h"
#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"
#include "services/ModesService.h"
#include "services/NetworkService.h"

BuildExtension *Build = nullptr;

BuildExtension::BuildExtension() : ExtensionModule("Build")
{
    Build = this;
}

void BuildExtension::ready()
{
    (*config)["download"] = std::string(Device.repository).append("/releases");
    Device.transmit(*config, name);
    delete config;
    config = nullptr;
}

#endif // EXTENSION_BUILD
