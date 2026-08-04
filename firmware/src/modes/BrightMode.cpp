#if MODE_BRIGHT

#include "modes/BrightMode.h"

#include "services/DisplayService.h"

void BrightMode::begin() { Display.fillFrame(UINT8_MAX); }

#endif // MODE_BRIGHT
