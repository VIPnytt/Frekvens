# 🎛️ Modes

## 🎬 Animation

Create simple animations and show it off on the display.

Use the [Web app](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-web-app) to get started.

> The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-microphone) extension can be used to play/pause the animations.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_ANIMATION=true
```

> Enabled by default.

> The [mode generator](https://github.com/VIPnytt/Frekvens/blob/main/extra/Python/csvModeGenerator.py) Python script converts animations into reusable modes.

> The [animation splitter](https://github.com/VIPnytt/Frekvens/blob/main/tools/csvAnimationSplitter.py) Python script separates exported animations into individual [drawings](#-draw).

See also [Draw](#-draw).

## ⬇️ Arrow

Inspired by the original *IKEA Frekvens* mode.

> The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-microphone) extension can be used to play/pause the arrow, just like on the original *IKEA Frekvens* device.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_ARROW=true
```

> Enabled by default.

## 💻 Art-Net

DMX over UDP/IP, stream in real-time over Wi-Fi.

Client: `example.local:6454`

> The [csv streamer](https://github.com/VIPnytt/Frekvens/blob/main/tools/csvStreamer.py) Python script streams custom content such as [animations](#-animation) and [drawings](#-draw).

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_ARTNET=true
```

> Enabled by default.

See also [Distributed Display Protocol](#-distributed-display-protocol) and [E1.31](#-e131).

## 0️⃣ Binary clock

Binary clock, counts seconds since midnight.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_BINARYCLOCK=true
```

> Enabled by default.

See also [Binary epoch](#1%EF%B8%8F⃣-binary-epoch).

## 1️⃣ Binary epoch

Binary clock, counts seconds since 1970.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_BINARYEPOCH=true
```

> Enabled by default.

See also [Binary clock](#0%EF%B8%8F⃣-binary-clock).

## 🪟 Blinds

Inspired by the original *IKEA Frekvens* mode.

> The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-microphone) extension can be used to play/pause the blinds.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_BLINDS=true
```

> Enabled by default.

## 👁️ Blink

Use it as an alarm.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_BLINK=true
```

> Enabled by default.

See also [Bright](#-bright).

## 🕰️ Bold clock

Clock.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_BOLD=true
```

> Enabled by default.

See also [Small clock](#%EF%B8%8F-small-clock).

## 💥 Breakout clock

Inspired by the classic breakout game.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_BREAKOUT=true
```

> Enabled by default.

## 🌟 Bright

Use it as a lamp, if desired.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_BRIGHT=true
```

> Enabled by default.

See also [Blink](#%EF%B8%8F-blink).

## ⚪ Circle

Inspired by the original *IKEA Obegränsad* mode.

> The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-microphone) extension can be used to play/pause the circle.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_CIRCLE=true
```

> Enabled by default.

See also [Ring](#-ring).

## ⏳ Countdown

Start a countdown timer.

The display will blink when the timer reaches zero.

Use the [Web app](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-web-app) to start a new countdown, it can also be automated via [Home Assistant](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-home-assistant).

API payload examples:

```json
{
    "seconds": 600
}
```

```json
{
    "timestamp": "2025-07-10T13:09:00"
}
```

> The timestamp should be a valid ISO 8601 timestamp, without timezone suffix.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_COUNTDOWN=true
```

> Enabled by default.

See also [Blink](#%EF%B8%8F-blink).

## 💻 Distributed Display Protocol

Efficient Ethernet pixel data, stream in real-time over Wi-Fi.

Client: `example.local:4048`

> The [csv streamer](https://github.com/VIPnytt/Frekvens/blob/main/tools/csvStreamer.py) Python script streams custom content such as [animations](#-animation) and [drawings](#-draw).

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_DISTRIBUTEDDISPLAYPROTOCOL=true
```

> Enabled by default.

See also [Art-Net](#-art-net) and [E1.31](#-e131).

## 🎨 Draw

Create artwork and show it off on the display.

Use the [Web app](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-web-app) to get started.

> The [mode generator](https://github.com/VIPnytt/Frekvens/blob/main/extra/Python/csvModeGenerator.py) Python script converts drawings into reusable modes.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_DRAW=true
```

> Enabled by default.

## 💻 E1.31

Streaming ACN, send DMX512 data over Wi-Fi.

Client: `example.local:5568`

> The [csv streamer](https://github.com/VIPnytt/Frekvens/blob/main/tools/csvStreamer.py) Python script streams custom content such as [animations](#-animation) and [drawings](#-draw).

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_E131=true
```

> Enabled by default.

See also [Art-Net](#-art-net) and [Distributed Display Protocol](#-distributed-display-protocol).

## 📊 Equalizer

Inspired by the original *IKEA Frekvens* mode.

> The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-microphone) extension can be used to play/pause the equalizer, just like on the original *IKEA Frekvens* device.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_EQUALIZER=true
```

> Enabled by default.

## 🎆 Firework

Happy new year.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_FIREWORK=true
```

> Enabled by default.

## 🪰 Flies

Multiplayer fly-simulator.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_FLIES=true
```

> Enabled by default.

## 🌱 Game of life

Inspired by Conway's *Game of Life*.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_GAMEOFLIFE=true
```

> Enabled by default.

See also [Game of life clock](#-game-of-life-clock).

## 🌳 Game of life clock

Inspired by *Conway's Game of Life*

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_GAMEOFLIFECLOCK=true
```

> Enabled by default.

See also [Game of life](#-game-of-life).

## ✨ Glitter

Inspired by *snow* on old analog TVs.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_GLITTER=true
```

> Enabled by default.

## 🏡 Home Assistant weather

Smart-home integration

Requires an [Long-lived access token](https://my.home-assistant.io/redirect/profile_security/), set using the `HOMEASSISTANT_KEY` variable. Other variables are optional.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) examples:

```h
#define HOMEASSISTANT_KEY "secret"
```

```h
#define HOMEASSISTANT_PROTOCOL "http:"
#define HOMEASSISTANT_HOST "homeassistant.local"
#define HOMEASSISTANT_PORT 8123
#define HOMEASSISTANT_ENTITY "weather.forecast_home"
```

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_HOMEASSISTANT=true
```

> Enabled by default when `HOMEASSISTANT_KEY` is defined.

See also [Home Assistant](https://github.com/VIPnytt/Frekvens/wiki/Extensions#home-assistant) extension.

## 🌡️ Home thermometer

Smart-home integration.

Display the current indoor and outdoor temperatures.

Integrated into [Home Assistant](https://github.com/VIPnytt/Frekvens/wiki/Extensions#home-assistant).

API payload example:

```json
{
    "indoor": 21,
    "outdoor": 9
}
```

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define HOMETHERMOMETER_UNIT "°C"
```

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_HOMETHERMOMETER=true
```

> Enabled by default.

## 📈 Jagged waveform

Inspired by the original *IKEA Frekvens* mode.

> The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-microphone) extension can be used to play/pause the waveforms, just like on the original *IKEA Frekvens* device.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_JAGGEDWAVEFORM=true
```

> Enabled by default.

See also [Smooth waveform](#%EF%B8%8F-smooth-waveform) and [Waveform](#-waveform).

## 🍂 Leaf fall

Autumn-inspired.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_LEAFFALL=true
```

> Enabled by default.

## ☰ Lines

Inspired by the original *IKEA Obegränsad* mode.

> The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-microphone) extension can be used to play/pause the lines, just like on the original *IKEA Frekvens* device.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_LINES=true
```

> Enabled by default.

See also [Scan](#-scan).

## 🔵 Metaballs

Inspired by organic, blob-like shapes.

> The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-microphone) extension can be used to play/pause the movement of the shapes.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_METABALLS=true
```

> Enabled by default.

## 🔇 Noise

Inspired by the original *IKEA Obegränsad* mode.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_NOISE=true
```

> Enabled by default.

## ⛅ Open-Meteo

Weather service.

Updated every 15+ minutes, depending on location.

Commercial usage requires an [API-key](https://open-meteo.com/en/pricing), set using the `OPENMETEO_KEY` variable. Other URL [parameters](https://open-meteo.com/en/docs#api_documentation) such as `temperature_unit` can be set using the `OPENMETEO_PARAMETERS` variable.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) examples:

```h
#define OPENMETEO_KEY "secret"
```

```h
#define OPENMETEO_PARAMETERS "temperature_unit=celsius" // query
```

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_OPENMETEO=true
```

> Enabled by default when `LATITUDE` and `LONGITUDE` is defined.

## ☀️ Open Weather

Weather service.

Updated every 10 minutes.

Requires a [API-key](https://openweathermap.org/api), set using the `OPENWEATHER_KEY` variable. Other URL parameters from the [Current Weather Data](https://openweathermap.org/current) and [One Call](https://openweathermap.org/api/one-call-3) APIs such as `units` can be set using the `OPENWEATHER_PARAMETERS` variable.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) examples:

```h
#define OPENWEATHER_KEY "secret"
```

```h
#define OPENWEATHER_PARAMETERS "units=metric" // query
```

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_OPENWEATHER=true
```

> Enabled by default when `LATITUDE` and `LONGITUDE` is defined.

## 🏓 Ping-pong

Inspired by the classic ping-pong game.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_PINGPONG=true
```

> Enabled by default.

See also [Ping-pong clock](#-ping-pong-clock).

## 🎱 Ping-pong clock

Inspired by the classic ping-pong game.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_PINGPONGCLOCK=true
```

> Enabled by default.

See also [Ping-pong](#-ping-pong).

## 🔢 Pixel sequence

For tech-geeks, iterates through the pixels one by one in the exact same order as the pixels are addressed on the SPI-bus.

> The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-microphone) extension can be used to play/pause the sequence.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_PIXELSEQUENCE=true
```

> Enabled by default.

## ☔ Rain

Inspired by the original *IKEA Obegränsad* mode.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_RAIN=true
```

> Enabled by default.

## 💍 Ring

Inspired by the original *IKEA Frekvens* mode.

> The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-microphone) extension can be used to play/pause the ring, just like on the original *IKEA Frekvens* device.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_RING=true
```

> Enabled by default.

See also [Circle](#-circle).

## 🔍 Scan

Inspired by the original *IKEA Obegränsad* mode.

> The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-microphone) extension can be used to play/pause the scan.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_SCAN=true
```

> Enabled by default.

See also [Lines](#-lines).

## ⏲️ Small clock

Clock.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_SMALLCLOCK=true
```

> Enabled by default.

See also [Bold clock](#%EF%B8%8F-bold-clock).

## 〰️ Smooth waveform

Inspired by the original *IKEA Frekvens* mode.

> The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-microphone) extension can be used to play/pause the waveforms, just like on the original *IKEA Frekvens* device.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_SMOOTHWAVEFORM=true
```

> Enabled by default.

See also [Jagged waveform](#-jagged-waveform) and [Waveform](#-waveform).

## 🐍 Snake

Inspired by the classic snake-game.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_SNAKE=true
```

> Enabled by default.

See also [Snake clock](#%EF%B8%8F-snake-clock).

## 🕰️ Snake clock

Inspired by the classic snake-game.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_SNAKECLOCK=true
```

> Enabled by default.

See also [Snake](#-snake).

## ⭐ Stars

Inspired by the original *IKEA Obegränsad* mode.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_STARS=true
```

> Enabled by default.

## 🅰️ Ticker

Rolling text.

Use the [Web app](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-web-app) to set a message, or automate it via [Home Assistant](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-home-assistant).

API payload example:

```json
{
    "font": "Mini",
    "message": "Hello world!"
}
```

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_TICKER=true
```

> Enabled by default.

See also [Message](https://github.com/VIPnytt/Frekvens/wiki/Extensions#message).

## ⏱️ Ticking clock

Clock with a seconds indicator.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_TICKINGCLOCK=true
```

> Enabled by default.

## 🔊 Waveform

Inspired by the two original *IKEA Frekvens* modes.

> The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-microphone) extension can be used to play/pause the waves, just like on the original *IKEA Frekvens* device.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_WAVEFORM=true
```

> Enabled by default.

See also [Jagged waveform](#-jagged-waveform) and [Smooth waveform](#️-smooth-waveform).

## 🌍 World Weather Online

Weather service.

Updated every 10-15 minutes.

Order of precedence:

- `LATITUDE` and `LONGITUDE`.
- `LOCATION`

Requires a [API-key](https://www.worldweatheronline.com/weather-api/api/docs/), set using the `WORLDWEATHERONLINE_KEY` variable. Other URL [parameters](https://www.worldweatheronline.com/weather-api/api/docs/local-city-town-weather-api.aspx#http_param) can be set using the `WORLDWEATHERONLINE_PARAMETERS` variable.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) examples:

```h
#define WORLDWEATHERONLINE_KEY "secret"
```

```h
#define WORLDWEATHERONLINE_PARAMETERS "foo=bar" // query
```

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_WORLDWEATHERONLINE=true
```

> Enabled by default when either `LATITUDE` and `LONGITUDE`, or `LOCATION` is defined.

## ☀️ Wttr.in

Weather service.

Updated hourly.

Order of precedence:

- `LATITUDE` and `LONGITUDE`.
- `LOCATION`
- Auto-detect

[URL parameters](https://wttr.in/:help) such as `m` and `u` can be set using the `WTTRIN_PARAMETERS` variable.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define WTTRIN_PARAMETERS "m" // query
```

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_WTTRIN=true
```

> Enabled by default.

## 🌧️ Yr

Weather service.

Denmark, Finland, Norway and Sweeden uses [Nowcast](https://api.met.no/weatherapi/nowcast/2.0/documentation), which is updated every 5 minutes. The rest of the world uses [Location Forecast](https://api.met.no/weatherapi/locationforecast/2.0/documentation).

URL parameters such as `altitude` can be set using the `YR_PARAMETERS` variable.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define YR_PARAMETERS "altitude=123" // query
```

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
MODE_YR=true
```

> Enabled by default when `LATITUDE` and `LONGITUDE` is defined.
