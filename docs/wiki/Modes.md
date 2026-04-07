# 🎛️ Modes

**Clocks:** [Binary clock](#0%EF%B8%8F⃣-binary-clock) | [Binary epoch](#1%EF%B8%8F⃣-binary-epoch) | [Breakout clock](#-breakout-clock) | [Clock](#️-clock) | [Game of Life](#-game-of-life) | [Ping-pong](#-ping-pong) | [Snake](#️-snake)

**Games:** [Breakout clock](#-breakout-clock) | [Game of life](#-game-of-life) | [Ping-pong](#-ping-pong)| [Snake](#-snake)

**IKEA Frekvens:** [Arrow](#️-arrow) | [Blinds](#-blinds) | [Equalizer](#-equalizer) | [Jagged Waveform](#-jagged-waveform) | [Ring](#-ring) | [Smooth waveform](#️-smooth-waveform)

**IKEA Obegränsad:** [Circle](#-circle) | [Lines](#-lines) | [Noise](#-noise) | [Rain](#-rain) | [Scan](#-scan) | [Stars](#-stars)

**Interactive:** [Animation](#-animation) | [Countdown](#-countdown) | [Draw](#-draw) | [Flies](#-flies) | [Ticker](#🅰%EF%B8%8F-ticker)

**Smart-home:** [Home Thermometer](#️-home-thermometer) | [Stream](#-stream) | [Weather](#-weather)

**Miscellaneous:** [Blink](#️-blink) | [Bright](#-bright) | [Firework](#-firework) | [Glitter](#-glitter) | [Leaf fall](#-leaf-fall) | [Metaballs](#-metaballs) | [Pixel sequence](#-pixel-sequence) | [Waveform](#-waveform)

## 🎬 Animation

Create simple animations and show it off on the display.

Use the [Web app](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-web-app) to get started.

> [!TIP]
> The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-microphone) extension can be used to play/pause the animations.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_ANIMATION='true'
```

See also [Draw](#-draw).

## ⬇️ Arrow

Inspired by the original *IKEA Frekvens* mode.

> [!TIP]
> The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-microphone) extension can be used to play/pause the arrow, just like on a stock *IKEA Frekvens* device.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_ARROW='true'
```

## 0️⃣ Binary clock

Binary clock, counts seconds since midnight.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_BINARYCLOCK='true'
```

See also [Binary epoch](#1%EF%B8%8F⃣-binary-epoch).

## 1️⃣ Binary epoch

Binary clock, counts seconds since 1970.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_BINARYEPOCH='true'
```

See also [Binary clock](#0%EF%B8%8F⃣-binary-clock).

## 🪟 Blinds

Inspired by the original *IKEA Frekvens* mode.

> [!TIP]
> The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-microphone) extension can be used to play/pause the blinds.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_BLINDS='true'
```

## 👁️ Blink

Use it as an alarm.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_BLINK='true'
```

See also [Bright](#-bright).

## 💥 Breakout clock

Inspired by the classic breakout game.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_BREAKOUT='true'
```

## 🌟 Bright

Use it as a lamp, if desired.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_BRIGHT='true'
```

See also [Blink](#%EF%B8%8F-blink).

## ⚪ Circle

Inspired by the original *IKEA Obegränsad* mode.

> [!TIP]
> The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-microphone) extension can be used to play/pause the circle.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_CIRCLE='true'
```

See also [Ring](#-ring).

## ⏱️ Clock

Clock with font selection and optional seconds indicator.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_CLOCK='true'
```

## ⏳ Countdown

Start a countdown timer.

The display will blink when the timer reaches zero.

Use the [Web app](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-web-app) to start a new countdown, it can also be automated via [Home Assistant](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-home-assistant).

API payload examples:

```json
{
    "time": 600
}
```

```json
{
    "timestamp": "2025-12-02T18:39:00"
}
```

> [!NOTE]
> The expected timestamp format is an ISO 8601 string without timezone suffix.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_COUNTDOWN='true'
```

See also [Blink](#%EF%B8%8F-blink).

## 🎨 Draw

Create artwork and show it off on the display.

Use the [Web app](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-web-app) to get started.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_DRAW='true'
```

## 📊 Equalizer

Inspired by the original *IKEA Frekvens* mode.

> [!TIP]
> The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-microphone) extension can be used to play/pause the equalizer, just like on a stock *IKEA Frekvens* device.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_EQUALIZER='true'
```

## 🎆 Firework

Happy new year.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_FIREWORK='true'
```

## 🪰 Flies

Multiplayer fly-simulator.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_FLIES='true'
```

## 🌱 Game of life

Inspired by Conway’s *Game of Life*. Comes with an optional clock.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_GAMEOFLIFE='true'
```

## ✨ Glitter

Inspired by *snow* on old analog TVs.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_GLITTER='true'
```

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

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_HOMETHERMOMETER='true'
```

## 📈 Jagged waveform

Inspired by the original *IKEA Frekvens* mode.

> [!TIP]
> The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-microphone) extension can be used to play/pause the waveforms, just like on a stock *IKEA Frekvens* device.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_JAGGEDWAVEFORM='true'
```

See also [Smooth waveform](#%EF%B8%8F-smooth-waveform) and [Waveform](#-waveform).

## 🍂 Leaf fall

Autumn-inspired.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_LEAFFALL='true'
```

## ☰ Lines

Inspired by the original *IKEA Obegränsad* mode.

> [!TIP]
> The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-microphone) extension can be used to play/pause the lines, just like on a stock *IKEA Frekvens* device.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_LINES='true'
```

See also [Scan](#-scan).

## 🔵 Metaballs

Inspired by organic, blob-like shapes.

> [!TIP]
> The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-microphone) extension can be used to play/pause the movement of the shapes.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_METABALLS='true'
```

## 🔇 Noise

Inspired by the original *IKEA Obegränsad* mode.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_NOISE='true'
```

## 🏓 Ping-pong

Inspired by the classic ping-pong game. Comes with an optional clock.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_PINGPONG='true'
```

## 🔢 Pixel sequence

For tech enthusiasts, it iterates through the pixels one by one, following the SPI bus addressing order.

> [!TIP]
> The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-microphone) extension can be used to play/pause the sequence.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_PIXELSEQUENCE='true'
```

## ☔ Rain

Inspired by the original *IKEA Obegränsad* mode.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_RAIN='true'
```

## 💍 Ring

Inspired by the original *IKEA Frekvens* mode.

> [!TIP]
> The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-microphone) extension can be used to play/pause the ring, just like on a stock *IKEA Frekvens* device.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_RING='true'
```

See also [Circle](#-circle).

## 🔍 Scan

Inspired by the original *IKEA Obegränsad* mode.

> [!TIP]
> The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-microphone) extension can be used to play/pause the scan.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_SCAN='true'
```

See also [Lines](#-lines).

## 〰️ Smooth waveform

Inspired by the original *IKEA Frekvens* mode.

> [!TIP]
> The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-microphone) extension can be used to play/pause the waveforms, just like on a stock *IKEA Frekvens* device.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_SMOOTHWAVEFORM='true'
```

See also [Jagged waveform](#-jagged-waveform) and [Waveform](#-waveform).

## 🐍 Snake

Inspired by the classic snake-game. Comes with an optional clock.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_SNAKE='true'
```

## ⭐ Stars

Inspired by the original *IKEA Obegränsad* mode.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_STARS='true'
```

## 💻 Stream

Provides real-time pixel and lighting data over Wi-Fi, allowing the display to be driven live from external software and controllers.

Multiple widely used streaming protocols are supported:

- Art-Net – Real-time lighting control data over UDP/IP.
- Distributed Display Protocol (DDP) – Efficient pixel-oriented data streaming.
- E1.31 (sACN) – Network-based DMX512 data streaming.

Receivers:

- `example.local:4048`
- `example.local:5568`
- `example.local:6454`

> [!TIP]
> The [Stream csv](https://github.com/VIPnytt/Frekvens/blob/main/tools/src/frekvens/StreamCsv.py) Python script streams custom content such as [animations](#-animation) and [drawings](#-draw).

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_STREAM='true'
```

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

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_TICKER='true'
```

See also [Message](https://github.com/VIPnytt/Frekvens/wiki/Extensions#message).

## 🔊 Waveform

Inspired by the two original *IKEA Frekvens* modes.

> [!TIP]
> The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-microphone) extension can be used to play/pause the waves, just like on a stock *IKEA Frekvens* device.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_WAVEFORM='true'
```

See also [Jagged waveform](#-jagged-waveform) and [Smooth waveform](#️-smooth-waveform).

## ⛅ Weather

Providers available:

- [Google](https://github.com/VIPnytt/Frekvens/wiki/Weather#-google)
- [Home Assistant](https://github.com/VIPnytt/Frekvens/wiki/Weather#-home-assistant)
- [Open-Meteo](https://github.com/VIPnytt/Frekvens/wiki/Weather#-open-meteo)
- [Open Weather](https://github.com/VIPnytt/Frekvens/wiki/Weather#-open-weather)
- [World Weather Online](https://github.com/VIPnytt/Frekvens/wiki/Weather#-world-weather-online)
- [Wttr.in](https://github.com/VIPnytt/Frekvens/wiki/Weather#-wttrin)
- [Yr](https://github.com/VIPnytt/Frekvens/wiki/Weather#-yr)

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define LATITUDE "0.000"  // coordinate
#define LONGITUDE "0.000" // coordinate
#define LOCATION "city"
```

> [!NOTE]
> Provide 3-4 decimals for the most accurate weather reports. A small subset of providers also supports a location name, usually in the form of a city or village.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_WEATHER='true'
```

```ini
TEMPERATURE_UNIT='°C' # °C, °F or °K
```
