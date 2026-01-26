#pragma once

/**
 * https://github.com/VIPnytt/Frekvens/wiki
 */

/**
 * Device
 */
#if IKEA_FREKVENS
#include "config/IkeaFrekvens.h"
#elif IKEA_OBEGRANSAD
#include "config/IkeaObegransad.h"
#else
#error "Unsupported device."
#endif

/**
 * Accessories
 */
#include "config/infrared.h"
#include "config/microphone.h"
#include "config/photocell.h"
#include "config/rtc.h"

/**
 * Essentials
 */
#include "config/extensions.h"
#include "config/modes.h"
#include "config/services.h"
#include "config/version.h"
