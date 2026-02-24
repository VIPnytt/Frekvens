#if MODE_GLITTER

#include "modes/GlitterMode.h"

#include "config/constants.h" // NOLINT(misc-include-cleaner)
#include "services/DisplayService.h"

void GlitterMode::handle() { Display.setPixel(random(GRID_COLUMNS), random(GRID_ROWS), random(1, 1 << 8)); }

#endif // MODE_GLITTER
