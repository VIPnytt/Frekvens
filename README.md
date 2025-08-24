# 💡 Frekvens

**Frekvens** is a custom ESP32 mod for *IKEA Frekvens* and *Obegränsad* LED displays, adding dozens of new display modes, smart-home control, and real-time integrations — all fully open-source and customizable.

The modular firmware is designed for flexibility and can be adapted to third-party or custom display configurations.

## 📋 Table of Contents

- [Features](#-features)
- [Hardware](#%EF%B8%8F-hardware)
- [Getting started](#️-getting-started)
- [Upload to the device](#️-upload-to-the-device)
- [Smart-home integrations](#-smart-home-integrations)
- [API](#-api)
- [FAQ](#-faq)
- [Troubleshooting](#-troubleshooting)

## ✨ Features

Frekvens replicates all original display modes while introducing a range of new features, including smart-home integration, streaming content, interactive tools, and real-time services.

### Key Highlights

- **Web app**
- **47 display modes**:
  - Animations
  - Clocks
  - Interactive tools
  - Text scroller
  - Streaming content
  - Weather displays
- **16 extensions**, including:
  - Smart-home integrations
  - Notifications
  - Accessories
  - API interfaces
- **7 fonts**
- Extensive [documentation](https://github.com/VIPnytt/Frekvens/wiki)

## ⚙️ Hardware

### ESP32 board

Frekvens supports most Wi-Fi enabled ESP32 boards. For new installations, the `ESP32-S3` is recommended for its performance and broad compatibility. A compact board (such as [this](https://www.adafruit.com/product/5426) or [this](https://www.seeedstudio.com/XIAO-ESP32S3-p-5627.html)) are sufficient for most use cases.

Due to significant variation in ESP32 board layouts, the documentation references pin types rather than fixed pin numbers. For board-specific recommendations, first check the [discussions section](https://github.com/VIPnytt/Frekvens/discussions/categories/general) — many common configurations are already covered there. If you're still unsure, refer to the [documentation](https://github.com/VIPnytt/Frekvens/wiki) or open a new topic for clarification.

### Optional Accessories

If desired, extra hardware can be added:

- IR — remote control
- Microphone — sync up with the music
- RTC — Real-time clock

> [IKEA Frekvens](https://github.com/VIPnytt/Frekvens/wiki/IKEA-Frekvens) already has a built-in microphone.

### Wiring

Each display model has its own hardware setup guide:

- [IKEA Frekvens](https://github.com/VIPnytt/Frekvens/wiki/IKEA-Frekvens#-getting-started)
  - Unsolder the `U2` chip
  - Connect power, SPI and enable
  - Microphone input available at `U3` pin 7 or `U2` pin 11
- [IKEA Obegränsad](https://github.com/VIPnytt/Frekvens/wiki/IKEA-Obegransad#getting-started)
  - Unsolder the `U1` chip
  - Connect power, SPI and enable

## 🏗️ Getting started

For the smoothest setup, we recommend [PlatformIO IDE](https://platformio.org/platformio-ide) for [VS Code](https://code.visualstudio.com). PlatformIO also provides [integrations](https://platformio.org/install/integration) for a wide range of other editors and IDEs — use whichever environment you are most comfortable with.

### Download the source code

- **Via Git (recommended)** – In your IDE, select *Clone Git Repository* and enter:

  `https://github.com/vipnytt/frekvens.git`

- **From the Releases page** – If you prefer a direct download, grab the latest stable `.zip` or `.tar.gz` archive from the [Releases page](https://github.com/vipnytt/frekvens/releases).

### Dependencies

To build the Web UI, [Node.js](https://nodejs.org) is required. The LTS version is recommended, but any recent version will work.

### PlatformIO

Specify your board in the [platformio.ini](https://github.com/VIPnytt/Frekvens/blob/main/platformio.ini) file. If your board is not already listed, check the [PlatformIO Boards](https://registry.platformio.org/platforms/platformio/espressif32/boards) reference.

[platformio.ini](https://github.com/VIPnytt/Frekvens/blob/main/platformio.ini) example:

```ini
[env:seeed_xiao_esp32s3]
board = seeed_xiao_esp32s3
```

### File Structure

Configuration is split into shared and firmware-specific settings:

- **Shared variables** (used by the firmware, web app, and tools) are defined in [`.env`](https://github.com/VIPnytt/Frekvens/blob/main/.env)
- **Firmware-only** settings go in [`firmware/include/config/secrets.h`](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h)

This separation keeps common parameters in one place, while allowing firmware builds to have their own purpose-specific configuration.

### Environment Template

Add one if the following to [`.env`](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
# IKEA Frekvens
ENV_FREKVENS=''

# IKEA Obegränsad
ENV_OBEGRANSAD=''
```

Define pin assignments in [`secrets.h`](firmware/include/config/secrets.h):

```h
#pragma once

// GPIO pins
#define PIN_SCLK 1
#define PIN_MOSI 2
#define PIN_CS 3
#define PIN_EN 4
#define PIN_SW2 5

// IKEA Frekvens only
#define PIN_SW1 6
#define PIN_MIC 7
```

### Wi-Fi

Wi-Fi credentials can be defined in [`secrets.h`](firmware/include/config/secrets.h) or configured through the web UI.

```h
#define WIFI_SSID "name"
#define WIFI_KEY "secret"
```

If no credentials are set, the device enters access point mode on first startup. Holding any button during startup also activates AP mode.

### Weather

Coordinates are required for weather services:

```h
#define LATITUDE "0.000"
#define LONGITUDE "0.000"
```

### Additional options

Check out the [Documentation](https://github.com/VIPnytt/Frekvens/wiki) for more configuration options.

## ⬆️ Upload to the device

In the *PlatformIO* menu, there's a *"Upload"* button which uploads the firmware, and a *"Upload Filesystem Image"* button which uploads the Web app. You'll need to upload both.

### OTA Uploads

To update *Over-the-Air*, extend your environment:

```ini
extends = env:upload_ota
```

Specify upload details:

```ini
upload_protocol = espota
upload_port = example.local
;upload_flags = --auth=password
```

As an alternative, it's also possible to manually upload the `firmware.bin` and `spiffs.bin` files via the Web app.

## 🏠 Smart-home integrations

### Alexa

Control power and brightness via Amazon Alexa, using voice, app, or automation.

### Home Assistant

Home Assistant is deeply integrated and supports full control over power, brightness, modes, extensions, and more.

## 🤝 API

Four API interfaces are supported:

- [MQTT](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-mqtt)
- [RESTful](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-restful)
- [Server-Sent Events](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-server-sent-events)
- [WebSocket](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-websocket)

Refer to the [Documentation](https://github.com/VIPnytt/Frekvens/wiki) for supported endpoints and use cases.

## ❓ FAQ

### How do I change Wi-Fi network?

Hold any button during startup to activate the Wi-Fi hotspot. Connect to it with your phone or computer, and a configuration portal will open. From there, you can configure new network credentials, and the device will remember multiple networks.

## 🚧 Troubleshooting

### Where is the web UI?

First, ensure that the Web app has been uploaded to the device using the *"Upload Filesystem Image"* button in the PlatformIO menu. After a successful upload, the device will log its IP address and domain name to the terminal at boot. You can then access the web UI by entering this address into your browser.

### The display is flickering

Flicker or "snow" usually indicates EMI or poor wiring. Avoid jumper wires and breadboards. Refer to wiring recommendations in the device-specific setup guides.

If display content is otherwise intact, try lowering the [frame rate](https://github.com/VIPnytt/Frekvens/wiki/Services#-display).

### Unexpected Reboots

If the device is rebooting unexpectedly, this can be a symptom of a software or hardware issue. The ESP32’s USB port is suitable for firmware uploads and testing, but it is not designed to supply the high current levels that a display might draw during continuous operation.

If terminal logs include error messages, please [report an issue](https://github.com/VIPnytt/Frekvens/issues).
