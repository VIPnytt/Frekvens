#pragma once
/**
 * https://github.com/VIPnytt/Frekvens/wiki
 */
#include "secrets.h" // please put your custom definitions in the "secrets.h" file

/**
 * Deprecations
 */
#include "config/deprecated.h"

/**
 * Device
 */
#if !defined(ENV) && defined(ENV_FREKVENS)
#include "config/ikeaFrekvens.h"
#elif !defined(ENV) && defined(ENV_OBEGRANSAD)
#include "config/ikeaObegransad.h"
#elif !defined(ENV) && defined(PIN_SW1) && defined(PIN_SW2) && defined(PIN_MIC)
#include "config/ikeaFrekvens.h"
#elif !defined(ENV) && defined(PIN_SW2)
#include "config/ikeaObegransad.h"
#elif !defined(ENV)
#error "Unsupported configuration. Please ensure that the correct environment is defined."
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
#include "config/services.h"
#include "config/fonts.h"
#include "config/extensions.h"
#include "config/modes.h"
#include "config/version.h"
