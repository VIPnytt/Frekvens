#if MODE_GLITTER

#include "config/constants.h"
#include "modes/GlitterMode.h"
#include "services/DisplayService.h"

void GlitterMode::handle()
{
    Display.setPixel(random(GRID_COLUMNS), random(GRID_ROWS), random(1, 1 << 8));
}

#endif // MODE_GLITTER
