#include "config/constants.h"

#if MODE_GLITTER

#include "modes/GlitterMode.h"
#include "services/DisplayService.h"

void GlitterMode::handle()
{
    Display.setPixel(random(COLUMNS), random(ROWS), random(1, 1U << 8));
}

#endif // MODE_GLITTER
