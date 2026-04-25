# 🧩 Extensions

**Accessories:** [Button](#️-button) | [Infrared](#-infrared) | [Microphone](#️-microphone) | [Photocell](#-photocell) | [RTC](#-rtc)

**APIs:** [MQTT](#️-mqtt) | [RESTful](#️-restful) | [Server-Sent Events](#-server-sent-events) | [WebSocket](#-websocket)

**Interactive:** [Button](#️-button) | [Message](#-message) | [Playlist](#️-playlist) | [Signal](#️-signal) | [Web app](#-web-app)

**Smart-home:** [Alexa](#-alexa) | [Home Assistant](#-home-assistant) | [MQTT](#️-mqtt)

**Tools:** [Heap](#-heap) | [OTA](#️-ota) | [Screenshot](#️-screenshot)

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

> [!NOTE]
> An [*Amazon Alexa*](https://www.amazon.com/smart-home-devices/b?node=9818047011) device is required.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
EXTENSION_ALEXA='true'
```

See also [Home Assistant](#-home-assistant).

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

> [!TIP]
> Long press *any* button during startup to activate the [Wi-Fi hotspot](https://github.com/VIPnytt/Frekvens/wiki/Services#-connectivity).

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define PIN_SW1 1 // Power button
```

```h
#define PIN_SW2 2 // Mode button
```

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
EXTENSION_BUTTON='true'
```

See also [IKEA Frekvens](https://github.com/VIPnytt/Frekvens/wiki/IKEA-Frekvens) and [IKEA Obegränsad](https://github.com/VIPnytt/Frekvens/wiki/IKEA-Obegransad).

## 💾 Heap

Reports memory usage.

Integrated into the [Home Assistant](#-home-assistant) extensions.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
EXTENSION_HEAP='true'
```

## 🧬 Home Assistant

Smart-home integration.

Key supported features include:

- Power, brightness, and [mode](https://github.com/VIPnytt/Frekvens/wiki/Modes) selection.
- Control over [extensions](https://github.com/VIPnytt/Frekvens/wiki/Extensions) like the microphone and infrared receiver.
- Access to various statistics, debug data, and advanced settings.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
EXTENSION_HOMEASSISTANT='true'
```

> [!IMPORTANT]
> The Home Assistant [MQTT](https://www.home-assistant.io/integrations/mqtt) integration is required.

See also [Home Assistant](https://github.com/VIPnytt/Frekvens/wiki/Weather#-home-assistant) weather.

## 🔴 Infrared

By adding an [IR receiver](https://github.com/VIPnytt/Frekvens/wiki/Infrared-receiver), you can use a remote control, like a TV remote, to operate the device. This gives you the flexibility to use an existing remote you may already own.

- Display brightness: up/down
  - Philips: Volume ➕➖
  - Sony: Volume ➕➖
- Display power: toggle
  - Philips: Power ⏻
  - Sony: Power ⏻
- [Microphone](#%EF%B8%8F-microphone): toggle
  - Philips: Mute 🔇
  - Sony: Mute 🔇
- [Mode](https://github.com/VIPnytt/Frekvens/wiki/Modes): next/previous
  - Philips: Title ⏮️⏭️ Album ⏪⏩
  - Sony: Program ➕➖ Previous ⏮️ Next ⏭️ Rewind ⏪ Fast forward ⏩
- [Photocell](#-photocell): toggle
  - Philips: `DIM`
  - Sony: `SCENE`
- [Playlist](#%EF%B8%8F-playlist): start/stop
  - Philips: Play/Pause ⏯️ Stop ⏹️
  - Sony: Play ▶️ Pause ⏸️ Stop ⏹️

> [!TIP]
> Use the [Web app](#-web-app) or [Home Assistant](#-home-assistant) to activate or deactivate this input method.

API payload example:

```json
{
    "active": true
}
```

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define PIN_IR 3 // Receiver
```

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
EXTENSION_INFRARED='true'
```

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

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
EXTENSION_MESSAGE='true'
```

See also [Ticker](https://github.com/VIPnytt/Frekvens/wiki/Modes#ticker).

## 🎙️ Microphone

If the device isn’t already equipped with a [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Microphone), adding one gives many [modes](https://github.com/VIPnytt/Frekvens/wiki/Modes) the ability to react to sounds, eg. syncing animations up with the music.

> [!TIP]
> Use the [Web app](#-web-app) or [Home Assistant](#-home-assistant) to control this input method.

API payload example:

```json
{
    "active": true,
}
```

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define PIN_MIC 4 // Amplifier
```

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
EXTENSION_MICROPHONE='true'
```

Check out the [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Microphone) wiki for hardware instructions.

## ✉️ MQTT

API-endpoint.

Topic:

- Publishes: `frekvens/hostname/+`
- Subscribed: `frekvens/hostname/+/set`

API message example:

```json
{
    "key": "value"
}
```

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define MQTT_HOST "mqtt.local"
#define MQTT_PORT 1883
#define MQTT_USER "name"
#define MQTT_KEY "password"
```

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
EXTENSION_MQTT='true'
```

## ✈️ OTA

Over-the-Air updates, for ESP32.

User-friendly interface for uploading `firmware.bin` and `littlefs.bin` files manually.

Another option is directly from the IDE/editor via Wi-Fi using `espota`.

Configure in [platformio.ini](https://github.com/VIPnytt/Frekvens/blob/main/platformio.ini):

```ini
upload_protocol = espota
upload_port = frekvens.local
;upload_flags = --auth=secret
```

> [!NOTE]
> The optional password protection removes the ability to upload manually via the [Web app](#-web-app) user-interface.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
OTA_KEY='secret'
```

```ini
EXTENSION_OTA='true'
```

## 🔆 Photocell

By adding a [Photocell](https://github.com/VIPnytt/Frekvens/wiki/Photocell), you’ll get automatic ambient brightness adaption.

> [!TIP]
> Use the [Web app](#-web-app) or [Home Assistant](#-home-assistant) to control this input method.

API payload example:

```json
{
    "active": true,
}
```

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define PIN_LDR 5 // Bridge
```

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
EXTENSION_PHOTOCELL='true'
```

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

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
EXTENSION_PLAYLIST='true'
```

## ↔️ RESTful

Provides a *RESTful* API.

- Full:
  - Method: `GET`
  - URL: `http://frekvens.local/restful/`
- Module:
  - Method: `GET` or `PATCH`
  - URL: `http://frekvens.local/restful/module`

API payload example:

```json
{
    "key": "value"
}
```

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
EXTENSION_RESTFUL='true'
```

See also [Server-Sent Events](#-server-sent-events).

## ⏰ RTC

By adding an [RTC-module](https://github.com/VIPnytt/Frekvens/wiki/Real-Time-Clock) you’ll get reliable clock, even without Wi-Fi connectivity.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
EXTENSION_RTC='true'
```

Check out the [Real-Time Clock](https://github.com/VIPnytt/Frekvens/wiki/Real-Time-Clock) wiki for hardware instructions.

## 📜 Screenshot

Take screenshots of the display.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
EXTENSION_SCREENSHOT='true'
```

See also [Web app](#️-web-app).

## 📜 Server-Sent Events

Real-time event stream API.

Endpoint: `http://frekvens.local/server-sent%20events`

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
EXTENSION_SERVERSENTEVENTS='true'
```

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

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
EXTENSION_SIGNAL='true'
```

## 📱 Web app

Handles the `./webapp` user-interface located in the filesystem partition.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
EXTENSION_WEBAPP='true'
```

> [!NOTE]
> Requires [WebSocket](#-websocket).

## 📞 WebSocket

Handles the WebSocket protocol.

Endpoint: `ws://frekvens.local/websocket`

API message example:

```json
{
    "module": {
        "key": "value"
    }
}
```

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
EXTENSION_WEBSOCKET='true'
```
