#pragma once
/*
 * https://github.com/VIPnytt/Frekvens/wiki/Fonts
 */
#include "secrets.h" // please put your custom definitions in the "secrets.h" file

/*
 * Braille
 */
#ifndef FONT_BRAILLE
#define FONT_BRAILLE true
#endif

/*
 * Small
 *
 * Deprecated since v1.1.0.
 * This option will be removed in v2.0.0.
 * The font will always be included in the future.
 * To maintain forward compatibility, do not use this option.
 */
#if defined(FONT_SMALL) && !FONT_SMALL
#pragma message("WARNING: FONT_SMALL is deprecated and will be removed in the future.")
#elif !defined(FONT_SMALL)
#define FONT_SMALL true
#endif
