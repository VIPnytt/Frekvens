#include "main.h"
#include "services/DeviceService.h"

void setup()
{
    Device.begin();
}

void loop()
{
    Device.handle();
    vTaskDelay(1);
}
