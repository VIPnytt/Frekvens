#pragma once
/*
 * https://github.com/VIPnytt/Frekvens/wiki
 */
#include "secrets.h" // please put your custom definitions in the "secrets.h" file

/*
 * Device
 */
#if !defined(ENV) && defined(ENV_FREKVENS)
#include "config/frekvens.h"
#elif !defined(ENV) && defined(ENV_OBEGRANSAD)
#include "config/obegransad.h"
#elif !defined(ENV) && defined(PIN_SW1) && defined(PIN_SW2) && defined(PIN_MIC)
#include "config/frekvens.h"
#elif !defined(ENV) && defined(PIN_SW2)
#include "config/obegransad.h"
#elif !defined(ENV)
#error "Unsupported configuration. Please ensure that the correct environment is defined."
#endif

/*
 * Accessories
 */
#include "config/infrared.h"
#include "config/microphone.h"
#include "config/rtc.h"

/*
 * Essentials
 */
#include "config/services.h"
#include "config/fonts.h"
#include "config/extensions.h"
#include "config/modes.h"
#include "config/version.h"
