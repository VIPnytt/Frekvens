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

Providing a nice user-interface with details about every variable in the config files, as well as copy/paste options just in case the previous configuration files got lost in the past.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_BUILD=true
```

> Enabled by default.

See also [OTA](#%EF%B8%8F-ota).

## ⏺️ Button

Buttons allows physical control of the device.

For devices with 2 buttons:

- #1: Power button
  - Click: *Power on/off*
  - Hold: *Brightness up/down*
- #2 Mode button
  - Click: *Next [mode](https://github.com/VIPnytt/Frekvens/wiki/Modes)*
  - Hold: *Iterate between [modes](https://github.com/VIPnytt/Frekvens/wiki/Modes)*

When the *Mode button* is the only button, it serves a dual purpose:

- #2: Mode button
  - Click: *Power on/off*
  - Hold: *Iterate between [modes](https://github.com/VIPnytt/Frekvens/wiki/Modes)*

Events such as `short` and `long` press is also implemented in [Home Assistant](#-home-assistant) and available via the API.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_SW1 1 // Power button
```

```h
#define PIN_SW2 2 // Mode button
```

> Long press *any* button during startup to activate the [Wi-Fi hotspot](https://github.com/VIPnytt/Frekvens/wiki/Services#-connectivity).

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_BUTTON=true
```

> Enabled by default when at least one of `PIN_SW1` and `PIN_SW2` is defined.

See also [IKEA Frekvens](https://github.com/VIPnytt/Frekvens/wiki/IKEA-Frekvens) and [IKEA Obegränsad](https://github.com/VIPnytt/Frekvens/wiki/IKEA-Obegransad).

## 🧬 Home Assistant

Smart-home integration.

Key supported features include:

- Power, brightness, and [mode](https://github.com/VIPnytt/Frekvens/wiki/Modes) selection.
- Control over [extensions](https://github.com/VIPnytt/Frekvens/wiki/Extensions) like the microphone and infrared receiver.
- Access to various statistics, debug data, and advanced settings.

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

See also [Home Assistant weather](https://github.com/VIPnytt/Frekvens/wiki/Modes#home-assistant-weather) mode.

## 🔴 Infrared

By adding an [IR receiver](https://github.com/VIPnytt/Frekvens/wiki/Infrared-receiver), you can use a remote control, like a TV remote, to operate the device. This gives you the flexibility to use an existing remote you may already own.

- Display brightness: up/down
  - Philips: Volume ➕➖
  - Sony: Volume ➕➖
- Display power: on/off
  - Philips: Power ⏻
  - Sony: Power ⏻
- [Microphone](#%EF%B8%8F-microphone): on/off
  - Philips: Mute 🔇
  - Sony: Mute 🔇
- [Mode](https://github.com/VIPnytt/Frekvens/wiki/Modes): surfing
  - Philips: Title ⏮️⏭️ Album ⏪⏩
  - Sony: Program ➕➖ Previous ⏮️ Next ⏭️ Rewind ⏪ Fast forward ⏩
- [Photocell](#-photocell): on/off
  - Philips: `DIM`
  - Sony: `SCENE`
- [Playlist](#%EF%B8%8F-playlist): start/stop
  - Philips: Play/Pause ⏯️ Stop ⏹️
  - Sony: Play ▶️ Pause ⏸️ Stop ⏹️

Use the [Web app](#-web-app) or [Home Assistant](#-home-assistant) to activate or deactivate this input method.

API payload example:

```json
{
    "active": true
}
```

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_IR 3 // Receiver
```

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_INFRARED=true
```

> Enabled by default when `PIN_IR` is defined.

Check out the [Infrared](https://github.com/VIPnytt/Frekvens/wiki/Infrared-receiver) wiki for more info.

## 📢 Message

Display rolling notification text messages on-demand.

Use the [Web app](#-web-app) to send messages, or set up automations via [Home Assistant](#-home-assistant).

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

See also [Ticker](https://github.com/VIPnytt/Frekvens/wiki/Modes#ticker).

## 🎙️ Microphone

If the device isn’t already equipped with a [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Microphone), adding one gives many [modes](https://github.com/VIPnytt/Frekvens/wiki/Modes) the ability to react to sounds, eg. syncing animations up with the music.

Use the [Web app](#-web-app) or [Home Assistant](#-home-assistant) to control this input method.

API payload example:

```json
{
    "active": true,
}
```

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_MIC 4 // Amplifier
```

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_MICROPHONE=true
```

> Enabled by default.

Check out the [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Microphone) wiki for hardware instructions.

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

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define MQTT_HOST "example.local"
#define MQTT_PORT 1883 // port #
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

## 🔆 Photocell

By adding a [Photocell](https://github.com/VIPnytt/Frekvens/wiki/Photocell), you’ll get automatic ambient brightness adaption.

Use the [Web app](#-web-app) or [Home Assistant](#-home-assistant) to control this input method.

API payload example:

```json
{
    "active": true,
}
```

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_LDR 5 // Bridge
```

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_PHOTOCELL=true
```

> Enabled by default.

Check out the [Photocell](https://github.com/VIPnytt/Frekvens/wiki/Photocell) wiki for hardware instructions.

## ▶️ Playlist

Set up a playlist of [Modes](https://github.com/VIPnytt/Frekvens/wiki/Modes) that loops with a timer.

Integrated into the [Web app](#-web-app) and [Home Assistant](#-home-assistant) extensions.

API payload examples:

```json
{
    "active": true,
}
```

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

By adding an [RTC-module](https://github.com/VIPnytt/Frekvens/wiki/Real-Time-Clock) you’ll get reliable clock, even without Wi-Fi connectivity.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
EXTENSION_RTC=true
```

> Enabled by default when a [supported](https://github.com/VIPnytt/Frekvens/wiki/Real-Time-Clock#-supported-types) RTC-module is defined.

Check out the [Real-Time Clock](https://github.com/VIPnytt/Frekvens/wiki/Real-Time-Clock) wiki for hardware instructions.

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

Handles the `webapp` user-interface located on the ESP32’s *SPIFFS* partition.

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
