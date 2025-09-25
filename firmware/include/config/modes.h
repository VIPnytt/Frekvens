#pragma once
/**
 * https://github.com/VIPnytt/Frekvens/wiki/Modes
 */
#include "secrets.h" // please put your custom definitions in the "secrets.h" file

/**
 * Animation
 */
#if !defined(MODE_ANIMATION) && (EXTENSION_MQTTT || EXTENSION_RESTFUL || EXTENSION_WEBSOCKET)
#define MODE_ANIMATION true
#endif

/**
 * Arrow
 */
#ifndef MODE_ARROW
#define MODE_ARROW true
#endif

/**
 * Art-Net
 */
#ifndef MODE_ARTNET
#define MODE_ARTNET true
#endif

/**
 * Binary clock
 */
#ifndef MODE_BINARYCLOCK
#define MODE_BINARYCLOCK true
#endif

/**
 * Binary epoch
 */
#ifndef MODE_BINARYEPOCH
#define MODE_BINARYEPOCH true
#endif

/**
 * Blinds
 */
#ifndef MODE_BLINDS
#define MODE_BLINDS true
#endif

/**
 * Blink
 */
#ifndef MODE_BLINK
#define MODE_BLINK true
#endif

/**
 * Bold clock
 */
#ifndef MODE_BOLDCLOCK
#define MODE_BOLDCLOCK true
#endif

/**
 * Breakout clock
 */
#ifndef MODE_BREAKOUTCLOCK
#define MODE_BREAKOUTCLOCK true
#endif

/**
 * Bright
 */
#ifndef MODE_BRIGHT
#define MODE_BRIGHT true
#endif

/**
 * Circle
 */
#ifndef MODE_CIRCLE
#define MODE_CIRCLE true
#endif

/**
 * Countdown
 */
#ifndef MODE_COUNTDOWN
#define MODE_COUNTDOWN true
#endif

/**
 * Distributed Display Protocol
 */
#ifndef MODE_DISTRIBUTEDDISPLAYPROTOCOL
#define MODE_DISTRIBUTEDDISPLAYPROTOCOL true
#endif

/**
 * Draw
 */
#if !defined(MODE_DRAW) && (EXTENSION_MQTTT || EXTENSION_RESTFUL || EXTENSION_WEBSOCKET)
#define MODE_DRAW true
#endif

/**
 * E1.31
 */
#ifndef MODE_E131
#define MODE_E131 true
#endif

/**
 * Equalizer
 */
#ifndef MODE_EQUALIZER
#define MODE_EQUALIZER true
#endif

/**
 * Firework
 */
#ifndef MODE_FIREWORK
#define MODE_FIREWORK true
#endif

/**
 * Flies
 */
#if !defined(MODE_FLIES) && (EXTENSION_MQTTT || EXTENSION_RESTFUL || EXTENSION_WEBSOCKET)
#define MODE_FLIES true
#endif

/**
 * Game of Life
 */
#ifndef MODE_GAMEOFLIFE
#define MODE_GAMEOFLIFE true
#endif

/**
 * Game of Life clock
 */
#ifndef MODE_GAMEOFLIFECLOCK
#define MODE_GAMEOFLIFECLOCK true
#endif

/**
 * Glitter
 */
#ifndef MODE_GLITTER
#define MODE_GLITTER true
#endif

/**
 * Home Assistant weather
 */
// #define HOMEASSISTANT_HOST "homeassistant.local"
// #define HOMEASSISTANT_PORT 8123
// #define HOMEASSISTANT_KEY "secret"
// #define HOMEASSISTANT_ENTITY "weather.forecast_home"
#if !defined(MODE_HOMEASSISTANTWEATHER) && defined(HOMEASSISTANT_KEY)
#define MODE_HOMEASSISTANTWEATHER true
#endif

/**
 * Home thermometer
 */
// #define HOMETHERMOMETER_UNIT "°C"
#if !defined(MODE_HOMETHERMOMETER) && (EXTENSION_MQTTT || EXTENSION_RESTFUL || EXTENSION_WEBSOCKET)
#define MODE_HOMETHERMOMETER true
#endif

/**
 * Jagged waveform
 */
#ifndef MODE_JAGGEDWAVEFORM
#define MODE_JAGGEDWAVEFORM true
#endif

/**
 * Leaf fall
 */
#ifndef MODE_LEAFFALL
#define MODE_LEAFFALL true
#endif

/**
 * Lines
 */
#ifndef MODE_LINES
#define MODE_LINES true
#endif

/**
 * Metaballs
 */
#ifndef MODE_METABALLS
#define MODE_METABALLS true
#endif

/**
 * Noise
 */
#ifndef MODE_NOISE
#define MODE_NOISE true
#endif

/**
 * Open-Meteo
 *
 * https://open-meteo.com/en/docs#api-documentation
 */
// #define OPENMETEO_KEY "secret"
// #define OPENMETEO_PARAMETERS "elevation=123"
#if !defined(MODE_OPENMETEO) && defined(LATITUDE) && defined(LONGITUDE)
#define MODE_OPENMETEO true
#endif

/**
 * OpenWeather
 *
 * https://openweathermap.org/api/one-call-3#start
 */
// #define OPENWEATHER_KEY "secret"
// #define OPENWEATHER_PARAMETERS "units=metric"
#if !defined(MODE_OPENWEATHER) && defined(OPENWEATHER_KEY) && defined(LATITUDE) && defined(LONGITUDE)
#define MODE_OPENWEATHER true
#endif

/**
 * Ping-Pong
 */
#ifndef MODE_PINGPONG
#define MODE_PINGPONG true
#endif

/**
 * Ping-Pong clock
 */
#ifndef MODE_PINGPONGCLOCK
#define MODE_PINGPONGCLOCK true
#endif

/**
 * Pixel sequence
 */
#ifndef MODE_PIXELSEQUENCE
#define MODE_PIXELSEQUENCE true
#endif

/**
 * Rain
 */
#ifndef MODE_RAIN
#define MODE_RAIN true
#endif

/**
 * Ring
 */
#ifndef MODE_RING
#define MODE_RING true
#endif

/**
 * Scan
 */
#ifndef MODE_SCAN
#define MODE_SCAN true
#endif

/**
 * Small clock
 */
#ifndef MODE_SMALLCLOCK
#define MODE_SMALLCLOCK true
#endif

/**
 * Smooth waveform
 */
#ifndef MODE_SMOOTHWAVEFORM
#define MODE_SMOOTHWAVEFORM true
#endif

/**
 * Snake
 */
#ifndef MODE_SNAKE
#define MODE_SNAKE true
#endif

/**
 * Snake clock
 */
#ifndef MODE_SNAKECLOCK
#define MODE_SNAKECLOCK true
#endif

/**
 * Stars
 */
#ifndef MODE_STARS
#define MODE_STARS true
#endif

/**
 * Ticker
 */
#ifndef MODE_TICKER
#define MODE_TICKER true
#endif

/**
 * Ticking clock
 */
#ifndef MODE_TICKINGCLOCK
#define MODE_TICKINGCLOCK true
#endif

/**
 * Waveform
 */
#ifndef MODE_WAVEFORM
#define MODE_WAVEFORM true
#endif

/**
 * World Weather Online
 *
 * https://www.worldweatheronline.com/weather-api/api/docs/local-city-town-weather-api.aspx#http_param
 */
// #define WORLDWEATHERONLINE_UNIT "C"
// #define WORLDWEATHERONLINE_KEY "secret"
// #define WORLDWEATHERONLINE_PARAMETERS "key=value"
#if !defined(MODE_WORLDWEATHERONLINE) && defined(WORLDWEATHERONLINE_KEY) && ((defined(LATITUDE) && defined(LONGITUDE)) || defined(LOCATION))
#define MODE_WORLDWEATHERONLINE true
#endif

/**
 * Wttr.in
 *
 * https://wttr.in/:help
 */
// #define WTTRIN_PARAMETERS "m"
#ifndef MODE_WTTRIN
#define MODE_WTTRIN true
#endif

/**
 * Yr
 *
 * https://api.met.no/weatherapi/locationforecast/2.0/documentation#Parameters
 * https://api.met.no/weatherapi/nowcast/2.0/documentation#Parameters
 */
// #define YR_PARAMETERS "altitude=123"
#if !defined(MODE_YR) && defined(LATITUDE) && defined(LONGITUDE)
#define MODE_YR true
#endif
