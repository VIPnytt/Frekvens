#if MODE_BRIGHT

#include "modes/BrightMode.h"

#include "services/DisplayService.h"

/**
 * @brief Sets every display pixel to maximum brightness.
 */
void BrightMode::begin() { Display.fillFrame(UINT8_MAX); }

#endif // MODE_BRIGHT
