#pragma once
/**
 * https://github.com/VIPnytt/Frekvens/wiki/Services
 */
#include "secrets.h" // please put your custom definitions in the "secrets.h" file

/**
 * Device
 */
// #define NAME "Frekvens"

/**
 * Display
 */
// #define FRAME_RATE 60 // fps
// #define SPI_FREQUENCY 10000000 // Hz

/**
 * Network
 */
// #define HOSTNAME "frekvens"
// #define DOMAIN ".local"
// #define DNS1 "1.1.1.1"
// #define DNS2 "8.8.8.8"
// #define DNS3 "9.9.9.9"

/**
 * Serial
 */
// #define F_INFO
// #define F_DEBUG
// #define F_VERBOSE
// #define MONITOR_SPEED 115200 // b/s
#if defined(F_VERBOSE) && !defined(F_DEBUG)
#define F_DEBUG
#endif
#if defined(F_DEBUG) && !defined(F_INFO)
#define F_INFO
#endif
//

/**
 * Tasks
 */
// #define TASK_STACK_EXTENSIONS 4096 // bytes
// #define TASK_STACK_MODES 8192 // bytes

/**
 * Time server
 */
// #define NTP1 "pool.ntp.org"
// #define NTP2 "time.nist.gov"
// #define NTP3 "time.cloudflare.com"

/**
 * Time zone
 */
// #define TIME_ZONE_POSIX "UTC0"

/**
 * Weather
 */
// #define LATITUDE "0.000" // °
// #define LONGITUDE "0.000" // °
// #define LOCATION "city"

/**
 * Wi-Fi
 */
// #define WIFI_SSID "name"
// #define WIFI_KEY "secret"
// #define WIFI_SSID_HOTSPOT "name"
// #define WIFI_KEY_HOTSPOT "secret"
