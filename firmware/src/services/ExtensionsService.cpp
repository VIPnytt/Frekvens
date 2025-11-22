#include "services/DeviceService.h"
#include "services/DisplayService.h"
#include "services/ExtensionsService.h"
#include "services/ModesService.h"

void ExtensionsService::setup()
{
    for (ExtensionModule *extension : modules)
    {
        extension->setup();
    }
    ESP_LOGV(name, "setup complete");
}

void ExtensionsService::ready()
{
    for (ExtensionModule *extension : modules)
    {
        extension->ready();
    }
    xTaskCreate(&onTask, name, stackSize, nullptr, 1, &taskHandle);
    transmit();
}

const std::vector<ExtensionModule *> &ExtensionsService::getAll() const
{
    return modules;
}

void ExtensionsService::transmit()
{
    JsonDocument doc;
    JsonArray list = doc["list"].to<JsonArray>();
    for (const ExtensionModule *extension : modules)
    {
        list.add(extension->name);
    }
    lastMillis = millis();
    Device.transmit(doc, name);
}

void ExtensionsService::onTask(void *parameter)
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

ExtensionsService &Extensions = Extensions.getInstance();
