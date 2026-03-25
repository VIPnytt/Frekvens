#include "services/ExtensionsService.h"

#include "services/DeviceService.h"

void ExtensionsService::configure()
{
    for (ExtensionModule *extension : modules)
    {
        extension->configure();
    }
}

void ExtensionsService::begin()
{
    for (ExtensionModule *extension : modules)
    {
        extension->begin();
    }
    xTaskCreate(&onTask, name, stackSize, nullptr, 1, &taskHandle);
    transmit();
}

const std::vector<ExtensionModule *> &ExtensionsService::getAll() const { return modules; }

void ExtensionsService::transmit()
{
    JsonDocument doc; // NOLINT(misc-const-correctness)
    JsonArray list{doc["list"].to<JsonArray>()};
    for (const ExtensionModule *extension : modules)
    {
        list.add(extension->name);
    }
    lastMillis = millis();
    Device.transmit(doc.as<JsonObjectConst>(), name);
}

void ExtensionsService::onTask(void *parameter) // NOLINT(misc-unused-parameters)
{
    for (;;)
    {
        for (ExtensionModule *extension : Extensions.getAll())
        {
            extension->handle();
        }
        vTaskDelay(1);
    }
}

ExtensionsService &ExtensionsService::getInstance()
{
    static ExtensionsService instance;
    return instance;
}

// NOLINTNEXTLINE(bugprone-throwing-static-initialization,cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables)
ExtensionsService &Extensions = ExtensionsService::getInstance();
