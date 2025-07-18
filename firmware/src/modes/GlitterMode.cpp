#include "config/constants.h"
#include "modes/GlitterMode.h"
#include "services/DisplayService.h"

void GlitterMode::handle()
{
    Display.setPixel(random(COLUMNS), random(ROWS), random(1, 1U << 8));
}
