#pragma once

/**
 * https://github.com/VIPnytt/Frekvens/wiki
 *
 * Definitions for the firmware section of the Frekvens project.
 * Updating? Go to Menu->Extensions->Build in the web interface to see recommended values for your setup.
 */

/*
 * Device
 */

//#define NAME "Obegraensad" //already defined in .env
#define NO_CHECK_4_UPDATES   // disable automatic check for firmware updates

/*
 * Display
 */

#define FRAME_RATE 50 // fps
#define SPI_FREQUENCY 10000000 // Hz

/*
 * Network settings
 */

//#define HOSTNAME "IKEA-PANEL-HACKMOD"     // may be already set in '.env' file
//#define DOMAIN ".local"                   // may be already set in '.env' file

#define DNS1 "192.168.11.250"             // FRITZ!Box (local Internet gateway)
#define DNS2 "8.8.8.8"
#define DNS3 "9.9.9.9"

#define NETWORK_SCAN_ATTEMPTS 5             // number of Wi-Fi scan attempts before reboot

/*
 * MQTT
 *   (needed for Home Assistant)
 */

 //#define MQTT_HOST "mqtt.local"
#define MQTT_HOST "192.168.11.11"
#define MQTT_PORT 1883
//#define MQTT_USER "mqtt_user"
//#define MQTT_KEY "secret"

/*
 * Serial line output 
 *   (may be better in platformio.ini)
 */

//#define F_INFO
//#define F_DEBUG
//#define F_VERBOSE
//#define MONITOR_SPEED 115200 // b/s

#if defined(F_DEBUG) && !defined(F_VERBOSE)
#define F_VERBOSE
#endif
//
#if defined(F_DEBUG) && !defined(F_INFO)
#define F_INFO
#endif
//

/*
 * Tasks
 */

#define TASK_STACK_EXTENSIONS 4096 // bytes
#define TASK_STACK_MODES 8192 // bytes

/*
 * Time server
 */

#define NTP1 "de.pool.ntp.org"
//#define NTP1 "pool.ntp.org"
//#define NTP2 "time.nist.gov"
//#define NTP3 "time.cloudflare.com"

/*
 * Time Zone
 * >> much better in '.env' 
 */

//#define TIME_ZONE_POSIX "UTC0"
//#define TIME_ZONE_POSIX "CET-1CEST,M3.5.0,M10.5.0/3"
//#define TIME_ZONE_IANA "Europe/Berlin"

/*
 * Modes
 */
#define MODE_DEFAULT "Ticking clock"

/*
 * Weather
 */

// LOCATION --------------------------| LAT ----- | LONG ----- | ZIP Code        
// Bad Kissingen (Altes Rathaus)      | 50.199864 |  10.076542 | D-97688
// Bad Waldsee (Altes Rathaus)        | 47.920710 |   9.754779 | D-88339
// Berlin (Rotes Rathaus)             | 52.518307 |  13.408642 | D-10178
// Darmstadt (Altes Rathaus)          | 49.872160 |   8.655899 | D-64283
// Frankfurt/M. (Altes Rathaus)       | 50.110546 |   8.681018 | D-60311
// Karlsruhe (Altes Rathaus)          | 49.008606 |   8.403475 | D-76133
// Schweinfurt (Altes Rathaus)        | 50.044731 |  10.234893 | D-97421
// Würzburg (Altes Rathaus)           | 49.793321 |   9.928625 | D-97070
// Santa Cruz de Tenerife (Town Hall) | 28.469825 | -16.254664 | ES-38004
//#define LATITUDE  "50.199864" // °
//#define LONGITUDE "10.076542" // °

#define LOCATION "D-97688" // wttr.in: City name or postal code

//define OPENMETEO_KEY "your_openmeteo_key" // optional, free registration at open-meteo.com
//#define OPENMETEO_PARAMETERS "hourly=temperature_2m,weathercode,precipitation,cloudcover&current_weather=true&temperature_unit=fahrenheit&windspeed_unit=m

//#define OPENWEATHER_KEY "your_openweather_key" // !NECESSARY! - free registration at openweathermap.org
//#define OPENWEATHER_PARAMETERS "units=metric&lang=de"

//#define WORLDWEATHERONLINE_KEY "your_wwonline_key" // !NECESSARY! - free registration at worldweatheronline.com
//#define WORLDWEATHERONLINE_PARAMETERS "num_of_days=1&lang=de"

//#define WTTRIN_PARAMETERS "m" // metric units
#define WTTRIN_INTERVAL 1000*60*20    // 20 minutes between updates

//#define YR_PARAMETERS "altitude=200" // altitude in meters

/*
 * Wi-Fi
 */

//#define WIFI_SSID "myWiFi-SSID"
//#define WIFI_KEY "secret"

#define WIFI_SSID_HOTSPOT "OBEGRAENSAD-LED"
#define WIFI_KEY_HOTSPOT "1233456789"
#define WIFI_COUNTRY "DE"  // ISO 3166-1 alpha-2 country code, default: "01"=worldwide
                           //    Supported country codes are "01"(world safe mode) "AT","AU","BE","BG","BR",
                           //    "CA","CH","CN","CY","CZ","DE","DK","EE","ES","FI","FR","GB","GR","HK","HR","HU",
                           //    "IE","IN","IS","IT","JP","KR","LI","LT","LU","LV","MT","MX","NL","NO","NZ","PL","PT",
                           //    "RO","SE","SI","SK","TW","US"

/*
 * Photocell extension (ambient light sensor)
 */

#define LDRGAMMA 0.4f     // caution: too high values (>0.7) may cause oscillations

/*
 * GPIO pins
 */

#define PIN_OE   12   // EN
#define PIN_MOSI 13   // DI
#define PIN_SCLK 11   // CLK
#define PIN_CS   10   // CLA
//#define PIN_SW1 35    // button mode 1
#define PIN_SW2  35   // button mode 2
#define PIN_LDR   2   // ambiente light sensor
//#define PIN_LED  47   // on-board LED (LED_BUILTIN)
