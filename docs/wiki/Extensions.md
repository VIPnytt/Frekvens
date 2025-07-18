# 🧩 Extensions

## 🎙️ Alexa

Smart-home integration.

Use app, voice control, or automations based on eg. *time* or *presence* to:

- Power on/off
- Adjust brightness

Enabled by default.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_ALEXA=true
```

See also [Home Assistant](#-home-assistant).

## 🏗️ Build

Firmware updates can be a hassle, the *Build* extension was *built* to change that.

Providing a nice user-interface with details about every variable in the config files, as well as copy/paste options just in case the previous project files got deleted in the past.

Enabled by default.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_BUILD=true
```

See also [OTA](#%EF%B8%8F-ota).

## ⏺️ Button

Buttons allows physical control of the device.

For devices with 2 buttons:

- Button #1:
  - Click: *Power on/off*
  - Hold: *Brightness up/down*
- Button #2:
  - Click: *Next [mode](Mode)*
  - Hold: *Iterate between [modes](Mode)*

For devices with only 1 button:

- Button #2:
  - Click: *Power on/off*
  - Hold: *Iterate between [modes](Mode)*

Events such as `short` and `long` press is also implemented in [Home Assistant](#-home-assistant) and available via the API.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_SW1 1 // GPIO #
```

```h
#define PIN_SW2 2 // GPIO #
```

**Note:** Long press *any* button during startup to activate the [Wi-Fi hotspot](Services#-wi-fi-hotspot).

Enabled by default when at least one of `PIN_SW1` and `PIN_SW2` is defined.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_BUTTON=true
```

See also [IKEA Frekvens](Frekvens.md) and [IKEA Obegränsad](Obegransad.md).

## 🤖 Home Assistant

Smart-home integration.

- Power on/off
- Brightness
- [Mode](Modes.md)
- [Countdown](Modes.md#countdown) timer
- [Home thermometer](Modes.md#home-thermometer)
- [Infrared](#-infrared) on/off
- [Message](#-message) notifications
- [Microphone](#%EF%B8%8F-microphone) on/off
- [Playlist](#%EF%B8%8F-playlist) start/stop
- [Ticker](Modes.md#ticker)
- Various stats, control and debug data.

Requires the [MQTT](#%EF%B8%8F-mqtt) extension.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define HOMEASSISTANT_TOPIC "homeassistant"
```

Enabled by default when the *[MQTT](#️-mqtt) extension* is enabled.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_HOMEASSISTANT=true
```

See also [Home Assistant weather](Modes.md#home-assistant-weather) mode.

## 🔴 Infrared

With an IR-receiver sensor, it's possible to use a remote control to control the device.

- Power on/off
- Brightness
- [Mode](Modes.md)
- [Microphone](#%EF%B8%8F-microphone) on/off
- [Playlist](#%EF%B8%8F-playlist) start/stop

Currently supported:

- Philips
- Sony

Use the *web app* to activate or deactivate this input method, it is also controllable via the [Home Assistant](#-home-assistant) extension.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_IR 3 // GPIO #
```

Enabled by default when `PIN_IR` is defined.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_INFRARED=true
```

Check out the [Infrared](Infrared.md) wiki for more info.

## 📢 Message

Display rolling notification text messages on-demand.

Use the *web app* to get started.

Integrated into [Home Assistant](#-home-assistant).

API example:

```json
{
    "font": "Mini",
    "message": "Hello world!",
    "repeat": 2
}
```

The `font` and `repeat` parameters is optional, if omitted, the last known value will be used.

Enabled by default.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_MESSAGE=true
```

See also [Ticker](Modes.md#ticker).

## 🎙️ Microphone

Many of the [modes](Modes.md) has the ability to react to sounds, eg. syncing animations up with the music.

Use the *web app* to activate or deactivate this input method, it is also controllable via the [Home Assistant](#-home-assistant) extension.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) examples:

```h
#define PIN_MIC 4 // GPIO #
```

```h
#define MIC_FLOOR 1024 // level
```

Enabled by default.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_MICROPHONE=true
```

Check out the [Microphone](Microphone.md) wiki for more info.

**Privacy note:** *there's an toggle in the user-interface to disable the microphone. It's also woth noting that the ESP32's analog inputs is capable of reading voltage levels only, not frequencies, therefore it's not possible to recording audio.*

## ✉️ MQTT

API-endpoint.

Topic:

- Publishes: `frekvens/example/+`
- Subscribed: `frekvens/example/+/set`

API message example:

```json
{
    "key": "value"
}
```

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) examples:

```h
#define MQTT_HOST "example.local"
#define MQTT_PORT 1883 // port #
```

```h
#define MQTT_USER "name"
#define MQTT_KEY "[REDACTED]"
```

Enabled by default when `MQTT_HOST` is defined.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_MQTT=true
```

## ✈️ OTA

Over-the-Air updates, for ESP32.

User-friendly interface for uploading `firmware.bin` and `spiffs.bin` files manually.

URL: `http://example.local/#/extensions/ota`

Another option is directly from the IDE/editor via Wi-Fi using `espota`.

[platformio.ini](https://github.com/VIPnytt/Frekvens/blob/main/platformio.ini) examples:

```ini
upload_protocol = espota
upload_port = example.local
```

```ini
upload_flags = --auth=REDACTED
```

There's also optional password protection, which disables the ability to upload manually via the [web app](#️-web-app) user-interface.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
OTA_KEY='REDACTED'
```

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define OTA_KEY_HASH "REDACTED"
```

Enabled by default.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_OTA=true
```

See also [Build](#%EF%B8%8F-build).

## ▶️ Playlist

Set up a playlist of [Modes](Modes.md) that loops with a timer.

Integrated into the [Button](#%EF%B8%8F-button) extension, long-press button #2 to activate. Can also be controlled via the [Home Assistant](#-home-assistant) extension.

Enabled by default.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_PLAYLIST=true
```

## 💻 RESTful

Provides a *RESTful* API.

- Full:
  - Method: `GET`.
  - URL: `http://example.local/api/get`
- Module:
  - Method: `GET` or `PATCH`.
  - URL prefix: `http://example.local/api/`

API payload example:

```json
{
    "key": "value"
}
```

Enabled by default.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_RESTFUL=true
```

## ⏰ RTC

Reliable clock, even without Wi-Fi connectivity.

Enabled by default when a [supported](RTC#-supported-types) RTC-module is defined.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_RTC=true
```

Check out the [RTC](RTC.md) wiki for more info.

## ☮️ Signal

Display drawings/icons on-demand.

API examples:

```json
{
    "bitmap": [
        "01100110",
        "01100110",
        "00000000",
        "11000011",
        "01111110"
    ],
    "duration": 30
}
```

```json
{
    "bitmap": [
        102,
        102,
        0,
        195,
        126
    ],
    "duration": 30
}
```

The `duration` parameters is optional, if omitted, the last known value will be used.

Enabled by default.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_SIGNAL=true
```

## 📱 Web app

Handles the `webapp` user-interface located on the ESP32's *SPIFFS* partition.

Canonical host check:

- Method: `GET`.
- URL: `http://example.local/canonical`

Enabled by default.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_WEBAPP=true
```

## 📞 WebSocket

Handles the WebSocket protocol.

Endpoint: `ws://example.local/ws`

API message example:

```json
{
    "module": {
        "key": "value"
    }
}
```

Enabled by default. It is also required for the frontend `webapp` user-interface to be functional.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_WEBSOCKET=true
```
