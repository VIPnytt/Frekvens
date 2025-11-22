#include "main.h"
#include "services/DeviceService.h"

void setup()
{
    Device.init();
}

void loop()
{
    Device.run();
    vTaskDelay(1);
}
