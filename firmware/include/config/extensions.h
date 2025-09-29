#pragma once

#include <soc/soc_caps.h>

/**
 * https://github.com/VIPnytt/Frekvens/wiki/Extensions
 */
#include "secrets.h" // please put your custom definitions in the "secrets.h" file

/**
 * Alexa
 */
#ifndef EXTENSION_ALEXA
#define EXTENSION_ALEXA true
#endif

/**
 * Build
 */
#ifndef EXTENSION_BUILD
#define EXTENSION_BUILD true
#endif

/**
 * Button
 */
#if !defined(EXTENSION_BUTTON) && (defined(PIN_SW1) || defined(PIN_SW2))
#define EXTENSION_BUTTON true
#endif

/**
 * IR
 */
#if !defined(EXTENSION_INFRARED) && defined(PIN_IR)
#define EXTENSION_INFRARED true
#endif

/**
 * Message
 */
#ifndef EXTENSION_MESSAGE
#define EXTENSION_MESSAGE true
#endif

/**
 * Mic
 */
#if !defined(EXTENSION_MICROPHONE) && defined(PIN_MIC)
#define EXTENSION_MICROPHONE true
#endif

/**
 * MQTT
 */
// #define MQTT_HOST "mqtt.local"
// #define MQTT_PORT 1883
// #define MQTT_USER "name"
// #define MQTT_KEY "secret"
#if !defined(EXTENSION_MQTT) && defined(MQTT_HOST)
#define EXTENSION_MQTT true
#endif

/**
 * OTA
 */
// #define OTA_KEY_HASH "md5" // Supersedes OTA_KEY
// #define OTA_KEY "secret"   // Superseded by OTA_KEY_HASH
#ifndef EXTENSION_OTA
#define EXTENSION_OTA true
#endif

/**
 * Photocell
 */
#if !defined(EXTENSION_PHOTOCELL) && defined(PIN_LDR)
#define EXTENSION_PHOTOCELL true
#endif

/**
 * Playlist
 */
#ifndef EXTENSION_PLAYLIST
#define EXTENSION_PLAYLIST true
#endif

/**
 * RESTful
 */
#ifndef EXTENSION_RESTFUL
#define EXTENSION_RESTFUL true
#endif

/**
 * RTC
 */
// #define RTC_DS1302
// #define RTC_DS1307
// #define RTC_DS3231
// #define RTC_DS3232
// #define RTC_DS3234
// #define RTC_PCF8563
#if !defined(EXTENSION_RTC) && (defined(RTC_DS1302) || defined(RTC_DS1307) || defined(RTC_DS3231) || defined(RTC_DS3232) || defined(RTC_DS3234) || defined(RTC_PCF8563))
#define EXTENSION_RTC true
#endif

/**
 * Server-Sent Events
 */
#ifndef EXTENSION_SERVERSENTEVENTS
#define EXTENSION_SERVERSENTEVENTS true
#endif

/**
 * Signal
 */
#ifndef EXTENSION_SIGNAL
#define EXTENSION_SIGNAL true
#endif

/**
 * WebSocket
 */
#ifndef EXTENSION_WEBSOCKET
#define EXTENSION_WEBSOCKET true
#endif

/**
 * Home Assistant
 */
// #define HOMEASSISTANT_TOPIC "homeassistant"
#if !defined(EXTENSION_HOMEASSISTANT) && EXTENSION_MQTT
#define EXTENSION_HOMEASSISTANT true
#endif

/**
 * Web app
 */
#if !defined(EXTENSION_WEBAPP) && EXTENSION_WEBSOCKET
#define EXTENSION_WEBAPP true
#endif
