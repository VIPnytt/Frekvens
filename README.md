# 💡 Frekvens

**Frekvens** is a ESP32-mod for [IKEA Frekvens](https://github.com/VIPnytt/Frekvens/wiki/IKEA-Frekvens) and [IKEA Obegränsad](https://github.com/VIPnytt/Frekvens/wiki/IKEA-Obegransad) LED displays, powered by the [SCT2024](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf) LED driver.

The modular firmware is designed for flexibility and can be adapted to third-party or custom display configurations.

## 📋 Table of Contents

- [Features](#-features)
- [Hardware](#%EF%B8%8F-hardware)
- [IDE setup](#-ide-setup)
- [Configuration](#️-configuration)
- [Uploading](#️-uploading)
- [Smart-home integrations](#-smart-home-integrations)
- [API](#-api)
- [FAQ](#-faq)
- [Troubleshooting](#-troubleshooting)

## ✨ Features

Frekvens replicates all original display modes while introducing a range of new features, including smart-home integration, streaming content, interactive tools, and real-time services.

### Key Highlights

- **Web-based interface**
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
  - External accessories
- **7 fonts**
- **4 API interfaces**
- Extensive [documentation](https://github.com/VIPnytt/Frekvens/wiki) available

## ⚙️ Hardware

### ESP32

Frekvens supports most Wi-Fi enabled ESP32 boards. For new installations, the `ESP32-S3` is recommended for its performance and broad compatibility. A compact board (such as [this](https://www.adafruit.com/product/5426) or [this](https://www.seeedstudio.com/XIAO-ESP32S3-p-5627.html)) are sufficient for most use cases. Boards with 8 MB flash memory are recommended, as they offer additional space for debugging and future expansion, though 4 MB is sufficient for basic operation.

Due to significant variation in ESP32 board layouts, the documentation references pin types rather than fixed pin numbers. For board-specific recommendations, first check the [discussions section](https://github.com/VIPnytt/Frekvens/discussions/categories/general) — many common configurations are already covered there. If you're still unsure, refer to the [documentation](https://github.com/VIPnytt/Frekvens/wiki) or open a new topic for clarification.

### Optional Accessories

If desired, extra hardware can be added:

- [Infrared receiver](https://github.com/VIPnytt/Frekvens/wiki/Infrared)
- [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Microphone)
- [Real-time clock (RTC)](https://github.com/VIPnytt/Frekvens/wiki/RTC)

> The [IKEA Frekvens](https://github.com/VIPnytt/Frekvens/wiki/IKEA-Frekvens) display includes a built-in microphone.

### Wiring

Each display model has its own hardware setup guide:

- [IKEA Frekvens](https://github.com/VIPnytt/Frekvens/wiki/IKEA-Frekvens#-getting-started)
  - Unsolder the `U2` chip
  - Connect power, SPI and enable
  - Microphone input available at `U3` pin 7 or `U2` pin 11
- [IKEA Obegränsad](https://github.com/VIPnytt/Frekvens/wiki/IKEA-Obegransad#getting-started)
  - Unsolder the `U1` chip
  - Connect power, SPI and enable

> Both devices require a logic level shifter.

## 💻 IDE setup

Any [IDE](https://docs.platformio.org/en/latest/integration/ide/index.html) supporting [PlatformIO](https://platformio.org) can be used. [Visual Studio Code](https://code.visualstudio.com) is recommended for new users.

To get started, choose **one** of the following setup methods:

1. **Clone the Repository**

    Use your IDE’s *Clone Git Repository* function and enter:
    `https://github.com/vipnytt/frekvens.git`

2. **Use a Stable Release**

   Visit the [Releases page](https://github.com/vipnytt/frekvens/releases) to download the latest stable source code as a `.zip` or `.tar.gz` archive.

3. **Manual Download**

   Alternatively, download the current development version using the green **Code** button on the [project page](https://github.com/vipnytt/frekvens).

### Node.js

To build the Web UI, [Node.js](https://nodejs.org) is required. The LTS version is recommended for most users, but any recent version will work.

## 🏗️ Configuration

### PlatformIO

Specify your board in the [platformio.ini](https://github.com/VIPnytt/Frekvens/blob/main/platformio.ini) file. If your board is not already listed, check the [PlatformIO Boards](https://registry.platformio.org/platforms/platformio/espressif32/boards) reference.

[platformio.ini](https://github.com/VIPnytt/Frekvens/blob/main/platformio.ini) example:

```ini
[env:seeed_xiao_esp32s3]
board = seeed_xiao_esp32s3
```

### File Structure

- Shared variables: [`.env`](https://github.com/VIPnytt/Frekvens/blob/main/.env)
- Firmware-specific settings: [`firmware/include/config/secrets.h`](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h)

While [`.env`](https://github.com/VIPnytt/Frekvens/blob/main/.env) includes a complete set of shared options by default, [`secrets.h`](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) is empty to encourage only necessary definitions. See the [Documentation](https://github.com/VIPnytt/Frekvens/wiki) for configuration options.

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

### Time zone

Set the IANA time zone in [`.env`](https://github.com/VIPnytt/Frekvens/blob/main/.env). Example:

```ini
TIME_ZONE_IANA='Etc/Universal'
```

[Time zone lookup](https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv)

### Wi-Fi

Wi-Fi credentials can be defined in [`secrets.h`](firmware/include/config/secrets.h) or configured through the web UI.

```h
#define WIFI_SSID "name"
#define WIFI_KEY "secret"
```

If no credentials are set, the device enters access point mode on first boot. Holding any button during startup also activates AP mode. This will not trigger if a known Wi-Fi signal is lost.

### Weather

Coordinates are required for weather services:

```h
#define LATITUDE "0.000"
#define LONGITUDE "0.000"
```

### Frame rate

Performance depends on SPI speed and signal quality:

```h
#define FRAME_RATE 40 // fps
#define SPI_FREQUENCY 10000000 // Hz
```

See [Display](https://github.com/VIPnytt/Frekvens/wiki/Services#-display) documentation for tuning guidance.

## ⬆️ Uploading

Use the PlatformIO menu to build and upload the firmware. Note that:

- *"Upload"* uploads `firmware.bin`

- *"Upload Filesystem Image"* uploads `spiffs.bin` (web app)

### OTA Uploads

To enable Over-the-Air (OTA) uploads, extend your environment:

```ini
extends = env:upload_ota
```

Specify upload details:

```ini
upload_protocol = espota
upload_port = example.local
;upload_flags = --auth=password
```

OTA uploads can also be performed manually via the Web app.

### Migration

Devices running certain OTA-libraries can use OTA migration:

```ini
extends = env:upload_ota_migration
```

Follow the standard OTA process, starting with the firmware upload. If either upload fails, a USB flash is required due to partition changes.

While wired upload is recommended the very first time, existing devices with alternative firmware are in some cases able to migrate OTA via Wi-Fi.

## 🏠 Smart-home integrations

### Alexa

Control power and brightness via Amazon Alexa, using voice, app, or automation.

### Home Assistant

Home Assistant integration supports full control over power, brightness, modes, extensions, and more.

More details available in the [Extensions](https://github.com/VIPnytt/Frekvens/wiki/Extensions) section.

## 🤝 API

Four API interfaces are supported:

- [RESTful](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-restful)
- [MQTT](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-mqtt)
- [Server-Sent Events](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-server-sent-events)
- [WebSocket](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-websocket)

Refer to the [Documentation](https://github.com/VIPnytt/Frekvens/wiki) for supported endpoints and use cases.

## ❓ FAQ

### Where is the web UI?

Ensure `spiffs.bin` is uploaded. The device logs its IP and domain to the terminal at boot. Access the interface through a browser.

### How do I change Wi-Fi network?

- If disconnected: hold a physical button during boot to activate AP mode.
- If connected: update credentials through the web UI.

### Is ESP8266 supported?

No. ESP8266 lacks the required performance for this project.

## 🚧 Troubleshooting

### The display is flickering

Flicker or "snow" usually indicates EMI or poor wiring. Avoid jumper wires and breadboards. Refer to wiring recommendations in the device-specific setup guides.

If display content is otherwise intact, try lowering the [frame rate](https://github.com/VIPnytt/Frekvens/wiki/Services#-display).

### Unexpected Reboots

The [IKEA Obegränsad](https://github.com/VIPnytt/Frekvens/wiki/IKEA-Obegransad) display draws up to *2.0 A @ 5.0 V* under full load. Use a USB power source rated for at least *10 W continuous*, and power the device using the original USB cable. Do not power the display through the ESP32’s USB port, as it is not designed to supply such current levels.

If terminal logs include error messages, please [report an issue](https://github.com/VIPnytt/Frekvens/issues).
