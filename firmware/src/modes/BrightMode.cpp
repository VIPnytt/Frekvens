#include "config/constants.h"

#if MODE_BRIGHT

#include "modes/BrightMode.h"
#include "services/DisplayService.h"

void BrightMode::wake()
{
    Display.clearFrame(UINT8_MAX);
}

#endif // MODE_BRIGHT
