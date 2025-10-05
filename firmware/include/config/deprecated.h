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
 * Deprecated since v1.2.0.
 * Scheduled for removal in v2.0.0.
 *
 * MODE_OPENMETRO has been replaced by MODE_OPENMETEO.
 * To ensure forward compatibility, update your configuration:
 * Replace MODE_OPENMETRO with MODE_OPENMETEO.
 */
#ifdef MODE_OPENMETRO
#define MODE_OPENMETEO MODE_OPENMETRO
#warning "'MODE_OPENMETRO' is deprecated since v1.2.0 and will be removed in v2.0.0. Use 'MODE_OPENMETEO' instead."
#endif

/**
 * OPENMETRO_KEY
 *
 * Deprecated since v1.2.0.
 * Scheduled for removal in v2.0.0.
 *
 * OPENMETRO_KEY has been replaced by OPENMETEO_KEY.
 * To ensure forward compatibility, update your configuration:
 * Replace OPENMETRO_KEY with OPENMETEO_KEY.
 */
#ifdef OPENMETRO_KEY
#define OPENMETEO_KEY OPENMETRO_KEY
#warning "'OPENMETRO_KEY' is deprecated since v1.2.0 and will be removed in v2.0.0. Use 'OPENMETEO_KEY' instead."
#endif

/**
 * OPENMETRO_PARAMETERS
 *
 * Deprecated since v1.2.0.
 * Scheduled for removal in v2.0.0.
 *
 * OPENMETRO_PARAMETERS has been replaced by OPENMETEO_PARAMETERS.
 * To ensure forward compatibility, update your configuration:
 * Replace OPENMETRO_PARAMETERS with OPENMETEO_PARAMETERS.
 */
#ifdef OPENMETRO_PARAMETERS
#define OPENMETEO_PARAMETERS OPENMETRO_PARAMETERS
#warning "'OPENMETRO_PARAMETERS' is deprecated since v1.2.0 and will be removed in v2.0.0. Use 'OPENMETEO_PARAMETERS' instead."
#endif

/**
 * PIN_EN
 *
 * Deprecated since v1.2.0.
 * Scheduled for removal in v2.0.0.
 *
 * PIN_EN has been replaced by PIN_OE.
 * To ensure forward compatibility, update your configuration:
 * Replace PIN_EN with PIN_OE.
 */
#ifdef PIN_EN
#define PIN_OE PIN_EN
#warning "'PIN_EN' is deprecated since v1.2.0 and will be removed in v2.0.0. Use 'PIN_OE' instead."
#endif
