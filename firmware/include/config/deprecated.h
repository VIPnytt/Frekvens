#pragma once
/**
 * https://github.com/VIPnytt/Frekvens/wiki
 */
#include "secrets.h" // please put your custom definitions in the "secrets.h" file

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
#warning "'FONT_SMALL' is deprecated since v1.1.0 and will be removed in v2.0.0. The font will always be included in the future."
#endif

/**
 * MODE_OPENMETRO
 *
 * MODE_OPENMETRO has been replaced by MODE_OPENMETEO.
 * To ensure forward compatibility, update your configuration:
 * Replace MODE_OPENMETRO with MODE_OPENMETEO.
 */
#ifdef MODE_OPENMETRO
#define MODE_OPENMETEO MODE_OPENMETRO
#endif

/**
 * OPENMETRO_KEY
 *
 * OPENMETRO_KEY has been replaced by OPENMETEO_KEY.
 * To ensure forward compatibility, update your configuration:
 * Replace OPENMETRO_KEY with OPENMETEO_KEY.
 */
#ifdef OPENMETRO_KEY
#define OPENMETEO_KEY OPENMETRO_KEY
#endif

/**
 * OPENMETRO_PARAMETERS
 *
 * OPENMETRO_PARAMETERS has been replaced by OPENMETEO_PARAMETERS.
 * To ensure forward compatibility, update your configuration:
 * Replace OPENMETRO_PARAMETERS with OPENMETEO_PARAMETERS.
 */
#ifdef OPENMETRO_PARAMETERS
#define OPENMETEO_PARAMETERS OPENMETRO_PARAMETERS
#endif

/**
 * PIN_EN
 *
 * PIN_EN has been replaced by PIN_OE.
 * To ensure forward compatibility, update your configuration:
 * Replace PIN_EN with PIN_OE.
 */
#ifdef PIN_EN
#define PIN_OE PIN_EN
#endif
