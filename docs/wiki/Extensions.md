# 🧩 Extensions

## 🎤 Alexa

Smart-home integration.

Use app, voice control, or automations based on eg. *time* or *presence* to:

- Power on/off
- Adjust brightness

In the *Alexa app*, navigate to:

1. Devices
2. Add Device
3. Other
4. Wi-Fi

> An [*Amazon Alexa*](https://www.amazon.com/smart-home-devices/b?node=9818047011) device is required.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_ALEXA=true
```

> Enabled by default.

See also [Home Assistant](#-home-assistant).

## 🏗️ Build

Firmware updates can be a hassle, the *Build* extension was *built* to change that.

Providing a nice user-interface with details about every variable in the config files, as well as copy/paste options just in case the previous project files got deleted in the past.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_BUILD=true
```

> Enabled by default.

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

> Long press *any* button during startup to activate the [Wi-Fi hotspot](Services#-wi-fi-hotspot).

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_BUTTON=true
```

> Enabled by default when at least one of `PIN_SW1` and `PIN_SW2` is defined.

See also [IKEA Frekvens](IKEA-Frekvens.md) and [IKEA Obegränsad](IKEA-Obegransad.md).

## 🧬 Home Assistant

Smart-home integration.

- Power on/off
- Brightness
- [Mode](Modes.md)
- [Countdown](Modes.md#countdown) timer
- [Home thermometer](Modes.md#home-thermometer)
- [Infrared](#-infrared)
- [Message](#-message) notifications
- [Microphone](#%EF%B8%8F-microphone)
- [Playlist](#%EF%B8%8F-playlist)
- [Ticker](Modes.md#ticker) text
- Various stats, control and debug data.

> The [*MQTT*](https://www.home-assistant.io/integrations/mqtt) integration is required.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define HOMEASSISTANT_TOPIC "homeassistant"
```

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_HOMEASSISTANT=true
```

> Enabled by default when the *[MQTT](#%EF%B8%8F-mqtt) extension* is enabled.

**Remove:**

Removes the *MQTT discovery packet*.

API payload example:

```json
{
    "action": "remove"
}
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

Use the Web app or [Home Assistant](#-home-assistant) to activate or deactivate this input method.

API payload example:

```json
{
    "active": true
}
```

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_IR 3 // GPIO #
```

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_INFRARED=true
```

> Enabled by default when `PIN_IR` is defined.

Check out the [Infrared](Infrared.md) wiki for more info.

## 📢 Message

Display rolling notification text messages on-demand.

Use the Web app to send messages, or set up automations via [Home Assistant](#-home-assistant).

API payload example:

```json
{
    "font": "Mini",
    "message": "Hello world!",
    "repeat": 2
}
```

The `font` and `repeat` parameters is optional, if omitted, the last known value will be used.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_MESSAGE=true
```

> Enabled by default.

See also [Ticker](Modes.md#ticker).

## 🎙️ Microphone

Many of the [modes](Modes.md) has the ability to react to sounds, eg. syncing animations up with the music.

Use the Web app or [Home Assistant](#-home-assistant) to control this input method.

API payload example:

```json
{
    "active": true,
}
```

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) examples:

```h
#define PIN_MIC 4 // GPIO #
```

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_MICROPHONE=true
```

> Enabled by default.

Check out the [Microphone](Microphone.md) wiki for hardware instructions.

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
#define MQTT_KEY "secret"
```

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_MQTT=true
```

> Enabled by default when `MQTT_HOST` is defined.

## ✈️ OTA

Over-the-Air updates, for ESP32.

User-friendly interface for uploading `firmware.bin` and `spiffs.bin` files manually.

URL: `http://example.local/#/extensions/ota`

Another option is directly from the IDE/editor via Wi-Fi using `espota`.

[platformio.ini](https://github.com/VIPnytt/Frekvens/blob/main/platformio.ini) examples:

```ini
upload_protocol = espota
upload_port = example.local
;upload_flags = --auth=secret
```

The optional password protection removes the ability to upload manually via the [Web app](#-web-app) user-interface.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
OTA_KEY='secret'
```

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define OTA_KEY_HASH "0123456789abcdef0123456789abcdef"
```

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_OTA=true
```

> Enabled by default.

See also [Build](#%EF%B8%8F-build).

## ▶️ Playlist

Set up a playlist of [Modes](Modes.md) that loops with a timer.

Integrated into the [Button](#%EF%B8%8F-button) extension, long-press button #2 to activate. Can also be controlled via the [Home Assistant](#-home-assistant) extension.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_PLAYLIST=true
```

> Enabled by default.

## ↔️ RESTful

Provides a *RESTful* API.

- Full:
  - Method: `GET`
  - URL: `http://example.local/api/`
- Module:
  - Method: `GET` or `PATCH`
  - URL prefix: `http://example.local/api/`

API payload example:

```json
{
    "key": "value"
}
```

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_RESTFUL=true
```

> Enabled by default.

See also [Server-Sent Events](#-server-sent-events).

## ⏰ RTC

Reliable clock, even without Wi-Fi connectivity.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_RTC=true
```

> Enabled by default when a [*supported*](RTC#-supported-types) RTC-module is defined.

Check out the [RTC](RTC.md) wiki for hardware instructions.

## 📜 Server-Sent Events

Real-time event stream API.

Endpoint: `http://example.local/api/events`

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_SERVERSENTEVENTS=true
```

> Enabled by default.

See also [RESTful](#️-restful).

## ☮️ Signal

Display drawings/icons on-demand.

API payload examples:

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

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_SIGNAL=true
```

> Enabled by default.

## 📱 Web app

Handles the `webapp` user-interface located on the ESP32's *SPIFFS* partition.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_WEBAPP=true
```

> Enabled by default when the *[WebSocket](#-websocket) extension* is enabled.

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

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_WEBSOCKET=true
```

> Enabled by default.
