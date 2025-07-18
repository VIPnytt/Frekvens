#include "modes/BrightMode.h"
#include "services/DisplayService.h"

void BrightMode::wake()
{
    Display.clear(UINT8_MAX);
}
