#include "main.h"

#include "services/DeviceService.h"

void setup() { Device.begin(); }

/**
 * @brief Processes device operations and yields the task for one tick.
 */
void loop()
{
    Device.handle();
    vTaskDelay(1U);
}
