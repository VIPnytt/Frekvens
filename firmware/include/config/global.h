#pragma once
/*
 * https://github.com/VIPnytt/Frekvens/wiki
 *
 * Put your custom definitions in the "secrets.h" file
 * This file is enforced globally, even across external libraries
 */

/*
 * crankyoldgit/IRremoteESP8266
 */
#ifndef DECODE_RC5
#define DECODE_RC5 true // Override
#endif
#ifndef DECODE_SONY
#define DECODE_SONY true // Override
#endif
#ifndef _IR_ENABLE_DEFAULT_
#define _IR_ENABLE_DEFAULT_ false // Override
#endif
