# 🎛️ Modes

## 🎬 Animation

Create simple animations and show it off on the display.

Use the *web app* to get started.

The [Microphone](Extensions.md#microphone) extension can be used to play/pause the animations.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_ANIMATION=true
```

There's a [Python script](../tree/main/tools/csvAnimationSplitter.py) available, to split exported animations into separate [drawings](#-draw)/frames/files. Exported animations can also be converted to full-fledged modes using the *Mode generator* [Python script](../tree/main/tools/csvModeGenerator.py).

See also [Draw](#-draw).

## ⬇️ Arrow

Inspired by the original *IKEA Frekvens* mode.

The [Microphone](Extensions.md#microphone) extension can be used to play/pause the arrow, just like on the original *IKEA Frekvens* device.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_ARROW=true
```

## 💻 Art-net

DMX over UDP/IP, stream in real-time over Wi-Fi.

Python template [script](../tree/main/tools/artnetTemplate.py).

Client: `example.local:6454`

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_ARTNET=true
```

There's a [Python script](../tree/main/tools/artnetTemplate.py) available, serving as a template for clients to connect.

See also [Distributed Display Protocol](#-distributed-display-protocol) and [E1.31](#-e131).

## 0️⃣ Binary clock

Binary clock, counts seconds since midnight.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_BINARYCLOCK=true
```

See also [Binary epoch](#1%EF%B8%8F⃣-binary-epoch).

## 1️⃣ Binary epoch

Binary clock, counts seconds since 1970.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_BINARYEPOCH=true
```

See also [Binary clock](#0%EF%B8%8F⃣-binary-clock).

## 🪟 Blinds

Inspired by the original *IKEA Frekvens* mode.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_BLINDS=true
```

The [Microphone](Extensions.md#microphone) extension can be used to play/pause the blinds.

## 👁️ Blink

Use it as an alarm.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_BLINK=true
```

See also [Bright](#-bright).

## 🕰️ Bold clock

Clock.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_BOLD=true
```

See also [Small clock](#%EF%B8%8F-small-clock).

## 💥 Breakout clock

Inspired by the classic breakout game.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_BREAKOUT=true
```

## 🌟 Bright

Use it as a lamp, if desired.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_BRIGHT=true
```

See also [Blink](#%EF%B8%8F-blink).

## ⚪ Circle

Inspired by the original *IKEA Obegränsad* mode.

The [Microphone](Extensions.md#microphone) extension can be used to play/pause the circle.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_CIRCLE=true
```

See also [Ring](#-ring).

## ⏳ Countdown

Start a countdown timer.

The display will blink when the timer reaches zero.

Use the *web app* to get started. Integrated into [Home Assistant](#-home-assistant).

API examples:

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

**Note:** *The timestamp should be a valid ISO 8601 timestamp, without timezone suffix.*

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_COUNTDOWN=true
```

See also [Blink](#%EF%B8%8F-blink).

## 💻 Distributed Display Protocol

Efficient Ethernet pixel data, stream in real-time over Wi-Fi.

There's a [Python script](../tree/main/tools/ddpTemplate.py) available, serving as a template for clients to connect.

Client: `example.local:4048`

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_DISTRIBUTEDDISPLAYPROTOCOL=true
```

There's a [Python script](../tree/main/tools/csvAnimationSplitter.py) available, serving as a template for clients to connect.

See also [Art-Net](#-art-net) and [E1.31](#-e131).

## 🎨 Draw

Create artwork and show it off on the display.

Use the *web app* to get started.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_DRAW=true
```

Exported drawing can be converted to full-fledged modes using the *Mode generator* [Python script](../tree/main/tools/csvModeGenerator.py).

## 💻 E1.31

Streaming ACN, send DMX512 data over Wi-Fi.

Python template [script](../tree/main/tools/e131Template.py).

Client: `example.local:5568`

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_E131=true
```

There's a [Python script](../tree/main/tools/e131Template.py) available, serving as a template for clients to connect.

See also [Art-Net](#-art-net) and [Distributed Display Protocol](#-distributed-display-protocol).

## 📊 Equalizer

Inspired by the original *IKEA Frekvens* mode.

The [Microphone](Extensions.md#microphone) extension can be used to play/pause the equalizer, just like on the original *IKEA Frekvens* device.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_EQUALIZER=true
```

## 🎆 Firework

Happy new year.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_FIREWORK=true
```

## 🪰 Flies

Multiplayer fly-simulator.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_FLIES=true
```

## 🌱 Game of life

Inspired by Conway's *Game of Life*.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_GAMEOFLIFE=true
```

See also [Game of life clock](#-game-of-life-clock).

## 🌳 Game of life clock

Inspired by *Conway's Game of Life*

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_GAMEOFLIFECLOCK=true
```

See also [Game of life](#-game-of-life).

## ✨ Glitter

Inspired by *snow* on old analog TVs.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_GLITTER=true
```

## 🏡 Home Assistant weather

Smart-home integration

Requires an [Long-lived access token](https://my.home-assistant.io/redirect/profile_security/), set using the `HOMEASSISTANT_KEY` variable. Other variables are optional.

[secrets.h](../tree/main/firmware/include/config/secrets.h) examples:

```h
#define HOMEASSISTANT_KEY "[REDACTED]"
```

```h
#define HOMEASSISTANT_HOST "homeassistant.local"
#define HOMEASSISTANT_PORT 8123 // port #
```

```h
#define HOMEASSISTANT_ENTITY "weather.forecast_home"
```

Enabled by default when `HOMEASSISTANT_KEY` is defined.

[.env](../tree/main/.env) example:

```ini
MODE_HOMEASSISTANT=true
```

See also [Home Assistant](Extensions.md#home-assistant) extension.

## 🌡️ Home thermometer

Smart-home integration.

Display the current indoor and outdoor temperatures.

Integrated into [Home Assistant](Extensions.md#home-assistant).

API example:

```json
{
    "indoor": 21,
    "outdoor": 9
}
```

[secrets.h](../tree/main/firmware/include/config/secrets.h) example:

```h
#define HOMETHERMOMETER_UNIT "°C"
```

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_HOMETHERMOMETER=true
```

## 📈 Jagged waveform

Inspired by the original *IKEA Frekvens* mode.

The [Microphone](Extensions.md#microphone) extension can be used to play/pause the waveforms, just like on the original *IKEA Frekvens* device.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_JAGGEDWAVEFORM=true
```

See also [Smooth waveform](#%EF%B8%8F-smooth-waveform) and [Waveform](#-waveform).

## 🍂 Leaf fall

Autumn-inspired.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_LEAFFALL=true
```

## ☰ Lines

Inspired by the original *IKEA Obegränsad* mode.

The [Microphone](Extensions.md#microphone) extension can be used to play/pause the lines, just like on the original *IKEA Frekvens* device.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_LINES=true
```

See also [Scan](#-scan).

## 🔇 Noise

Inspired by the original *IKEA Obegränsad* mode.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_NOISE=true
```

## 🚇 Open metro

Weather service.

Updated every 15+ minutes, depending on location.

Commercial usage requires an [API-key](https://open-meteo.com/en/pricing), set using the `OPENMETRO_KEY` variable. Other URL [parameters](https://open-meteo.com/en/docs#api_documentation) such as `temperature_unit` can be set using the `OPENMETRO_PARAMETERS` variable.

[secrets.h](../tree/main/firmware/include/config/secrets.h) examples:

```h
#define OPENMETRO_KEY "[REDACTED]"
```

```h
#define OPENMETRO_PARAMETERS "temperature_unit=celsius" // query
```

Enabled by default when `LATITUDE` and `LONGITUDE` is defined.

[.env](../tree/main/.env) example:

```ini
MODE_OPENMETRO=true
```

## ☀️ Open Weather

Weather service.

Updated every 10 minutes.

Requires a [API-key](https://openweathermap.org/api), set using the `OPENWEATHER_KEY` variable. Other URL parameters from the [Current Weather Data](https://openweathermap.org/current) and [One Call](https://openweathermap.org/api/one-call-3) APIs such as `units` can be set using the `OPENWEATHER_PARAMETERS` variable.

[secrets.h](../tree/main/firmware/include/config/secrets.h) examples:

```h
#define OPENWEATHER_KEY "[REDACTED]"
```

```h
#define OPENWEATHER_PARAMETERS "units=metric" // query
```

Enabled by default when `LATITUDE` and `LONGITUDE` is defined.

[.env](../tree/main/.env) example:

```ini
MODE_OPENWEATHER=true
```

## 🏓 Ping-pong

Inspired by the classic ping-pong game.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_PINGPONG=true
```

See also [Ping-pong clock](#-ping-pong-clock).

## 🎱 Ping-pong clock

Inspired by the classic ping-pong game.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_PINGPONGCLOCK=true
```

See also [Ping-pong](#-ping-pong).

## 🔢 Pixel sequence

For tech-nerds, iterates through the pixels one by one in the exact same order as the pixels are addressed on the SPI-bus.

The [Microphone](Extensions.md#microphone) extension can be used to play/pause the sequence.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_PIXELSEQUENCE=true
```

## ☔ Rain

Inspired by the original *IKEA Obegränsad* mode.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_RAIN=true
```

## 💍 Ring

Inspired by the original *IKEA Frekvens* mode.

The [Microphone](Extensions.md#microphone) extension can be used to play/pause the ring, just like on the original *IKEA Frekvens* device.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_RING=true
```

See also [Circle](#-circle).

## 🔍 Scan

Inspired by the original *IKEA Obegränsad* mode.

The [Microphone](Extensions.md#microphone) extension can be used to play/pause the scan.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_SCAN=true
```

See also [Lines](#-lines).

## ⏲️ Small clock

Clock.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_SMALLCLOCK=true
```

See also [Bold clock](#%EF%B8%8F-bold-clock).

## 〰️ Smooth waveform

Inspired by the original *IKEA Frekvens* mode.

The [Microphone](Extensions.md#microphone) extension can be used to play/pause the waveforms, just like on the original *IKEA Frekvens* device.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_SMOOTHWAVEFORM=true
```

See also [Jagged waveform](#-jagged-waveform) and [Waveform](#-waveform).

## 🐍 Snake

Inspired by the classic snake-game.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_SNAKE=true
```

See also [Snake clock](#%EF%B8%8F-snake-clock).

## 🕰️ Snake clock

Inspired by the classic snake-game.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_SNAKECLOCK=true
```

See also [Snake](#-snake).

## ⭐ Stars

Inspired by the original *IKEA Obegränsad* mode.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_STARS=true
```

## 🅰️ Ticker

Rolling text.

Use the *web app* to get started. Integrated into [Home Assistant](#-home-assistant).

API example:

```json
{
    "font": "Mini",
    "message": "Hello world!"
}
```

The `font` parameter is optional, if omitted, the last known value will be used.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_TICKER=true
```

See also [Message](Extensions.md#message).

## ⏱️ Ticking clock

Clock with a seconds indicator.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_TICKINGCLOCK=true
```

## 🔊 Waveform

Inspired by the two original *IKEA Frekvens* modes.

The [Microphone](Extensions.md#microphone) extension can be used to play/pause the waves, just like on the original *IKEA Frekvens* device.

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_WAVEFORM=true
```

See also [Jagged waveform](#-jagged-waveform) and [Smooth waveform](#️-smooth-waveform).

## 🌍 World Weather Online

Weather service.

Updated every 10-15 minutes.

Order of precedence:

- `LATITUDE` and `LONGITUDE`.
- `LOCATION`

Requires a [API-key](https://www.worldweatheronline.com/weather-api/api/docs/), set using the `WORLDWEATHERONLINE_KEY` variable. Other URL [parameters](https://www.worldweatheronline.com/weather-api/api/docs/local-city-town-weather-api.aspx#http_param) can be set using the `WORLDWEATHERONLINE_PARAMETERS` variable.

[secrets.h](../tree/main/firmware/include/config/secrets.h) examples:

```h
#define WORLDWEATHERONLINE_KEY "[REDACTED]"
```

```h
#define WORLDWEATHERONLINE_PARAMETERS "foo=bar" // query
```

Enabled by default when either `LATITUDE` and `LONGITUDE`, or `LOCATION` is defined.

[.env](../tree/main/.env) example:

```ini
MODE_WORLDWEATHERONLINE=true
```

## ☀️ Wttr.in

Weather service.

Updated hourly.

Order of precedence:

- `LATITUDE` and `LONGITUDE`.
- `LOCATION`
- Auto-detect

[URL parameters](https://wttr.in/:help) such as `m` and `u` can be set using the `WTTRIN_PARAMETERS` variable.

[secrets.h](../tree/main/firmware/include/config/secrets.h) example:

```h
#define WTTRIN_PARAMETERS "m" // query
```

Enabled by default.

[.env](../tree/main/.env) example:

```ini
MODE_WTTRIN=true
```

## 🌧️ Yr

Weather service.

Denmark, Finland, Norway and Sweeden uses [Nowcast](https://api.met.no/weatherapi/nowcast/2.0/documentation), which is updated every 5 minutes. The rest of the world uses [Location Forecast](https://api.met.no/weatherapi/locationforecast/2.0/documentation).

URL parameters such as `altitude` can be set using the `YR_PARAMETERS` variable.

[secrets.h](../tree/main/firmware/include/config/secrets.h) example:

```h
#define YR_PARAMETERS "altitude=123" // query
```

Enabled by default when `LATITUDE` and `LONGITUDE` is defined.

[.env](../tree/main/.env) example:

```ini
MODE_YR=true
```
