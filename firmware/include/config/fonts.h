#pragma once
/**
 * https://github.com/VIPnytt/Frekvens/wiki/Fonts
 */
#include "secrets.h" // please put your custom definitions in the "secrets.h" file

/**
 * Braille
 */
#ifndef FONT_BRAILLE
#define FONT_BRAILLE true
#endif

/**
 * FONT_SMALL
 *
 * Deprecated since v1.1.0.
 * Scheduled for removal in v2.0.0.
 *
 * Starting with v2.0.0, the small font will always be included.
 * To maintain forward compatibility, remove FONT_SMALL from your configuration.
 */
#ifdef FONT_SMALL
#pragma message("WARNING: 'FONT_SMALL' is deprecated since v1.1.0 and will be removed in v2.0.0. The font will always be included in the future.")
#endif
