# 💡 Frekvens

**Frekvens** is a custom ESP32 mod for *IKEA Frekvens* and *Obegränsad* LED displays, adding dozens of new display modes, smart-home control, and real-time integrations — all fully open-source and customizable.

The modular firmware is designed for flexibility and can be adapted to third-party or custom display configurations.

## 📋 Table of Contents

- [Features](#-features)
- [Hardware](#%EF%B8%8F-hardware)
- [Getting started](#%EF%B8%8F-getting-started)
- [Upload to the device](#️-upload-to-the-device)
- [Smart-home integrations](#-smart-home-integrations)
- [API](#-api)
- [FAQ](#-faq)
- [Troubleshooting](#-troubleshooting)

## ✨ Features

Frekvens replicates all original display modes while introducing a range of new features, including smart-home integration, streaming content, interactive tools, and real-time services.

### Key Highlights

- **43 display modes**:
  - Animations
  - Clocks
  - Interactive tools
  - Text scroller
  - Streaming content
  - Weather displays
- **17 extensions**:
  - Web app
  - Smart-home integrations
  - Notifications
  - Accessories
  - API interfaces
  - Miscellaneous
- **7 fonts**
- Extensive [documentation](https://github.com/VIPnytt/Frekvens/wiki)

## ⚙️ Hardware

### ESP32 board

Frekvens can run on any commonly available ESP32 board with Wi-Fi.

Because board layouts vary widely, the documentation refers to pin types rather than fixed pin numbers. For board-specific guidance, see the [discussions section](https://github.com/VIPnytt/Frekvens/discussions/categories/general) — many common configurations are already covered there. Further details are available in the [wiki](https://github.com/VIPnytt/Frekvens/wiki#-devices).

> [!TIP]
> If a new board is required, the `ESP32-S3` chip is recommended — for example [this board](https://www.adafruit.com/product/5426) or [this board](https://www.seeedstudio.com/XIAO-ESP32S3-p-5627.html). It offers excellent performance, and variants with 8 MB flash memory provide plenty of room for features and future expansion. Boards with 4 MB flash works fine too, for basic setups.

### Optional Accessories

If desired, extra hardware can be added:

- IR receiver — lets you control the device with a standard TV-style remote
- Microphone — syncs the display with music
- Photocell — automatic ambient brightness adaption
- RTC clock — keeps accurate time, even without internet access

> [!NOTE]
> [IKEA Frekvens](https://github.com/VIPnytt/Frekvens/wiki/IKEA-Frekvens) already has a built-in microphone.

### Wiring

Each display model has its own hardware setup guide:

- [IKEA Frekvens](https://github.com/VIPnytt/Frekvens/wiki/IKEA-Frekvens)
  - Desolder the `U2` chip
  - Connect power, SPI and `EN`
  - Microphone input available at `U3` pin 7 or `U2` pin 11
- [IKEA Obegränsad](https://github.com/VIPnytt/Frekvens/wiki/IKEA-Obegransad)
  - Desolder the `U1` chip
  - Connect power, SPI and `EN`

## 🏗️ Getting started

[PlatformIO IDE](https://platformio.org/platformio-ide) is required. It provides [integrations](https://platformio.org/install/integration) for a wide range of IDEs — use whichever editor you are most comfortable with.

### Download the source code

- **Via Git (recommended)** – In your IDE/editor, select *Clone Git Repository* and enter:

  `https://github.com/vipnytt/frekvens.git`

- **From the Releases page** – If you prefer a direct download, grab an `zip` or `tar.gz` archive from the [releases page](https://github.com/vipnytt/frekvens/releases/latest).

### Dependencies

To build the Web UI, also [Node.js](https://nodejs.org/en/download) is required. The latest *LTS* version is recommended, but any newer version will work.

### PlatformIO

Specify your board in the [platformio.ini](https://github.com/VIPnytt/Frekvens/blob/main/platformio.ini) file. If your board is not already listed, check the [PlatformIO Boards](https://registry.platformio.org/platforms/platformio/espressif32/boards) reference or the [pioarduino boards](https://github.com/pioarduino/platform-espressc6if32/tree/main/boards) database.

Configure in [platformio.ini](https://github.com/VIPnytt/Frekvens/blob/main/platformio.ini):

```ini
[env:seeed_xiao_esp32s3]
board = seeed_xiao_esp32s3 ; Board ID
```

### File Structure

Configuration is split into shared and firmware-specific settings:

- **Shared variables** used by the firmware and web app are defined in [`.env`](https://github.com/VIPnytt/Frekvens/blob/main/.env)
- **Firmware-only** settings go in [`firmware/include/config/secrets.h`](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h)

This separation keeps common parameters in one place, while allowing firmware builds to have their own purpose-specific configuration.

### Environment Template

Add one if the following to [`.env`](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
IKEA_FREKVENS='true' # IKEA Frekvens
```

```ini
IKEA_OBEGRANSAD='true' # IKEA Obegränsad
```

Define pin assignments in [`secrets.h`](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
// IKEA Frekvens
#define PIN_CS 1   // LAK
#define PIN_SCLK 2 // CLK
#define PIN_MOSI 3 // DA
#define PIN_OE 4   // EN
#define PIN_SW1 5  // SW1
#define PIN_SW2 6  // SW
#define PIN_MIC 7  // U3 pin 7
```

```h
// IKEA Obegränsad
#define PIN_CS 1   // CLA
#define PIN_SCLK 2 // CLK
#define PIN_MOSI 3 // DI
#define PIN_OE 4   // EN
#define PIN_SW2 5  // SW
```

### Wi-Fi

Define the Wi-Fi credentials in [`secrets.h`](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h).

```h
#define WIFI_SSID "name"
#define WIFI_KEY "secret"
```

> [!TIP]
> Additional networks can be configured later in the web UI, optionally via Wi-Fi hotspot if needed.

### Weather

Coordinates are required for most weather services:

```h
#define LATITUDE "0.000"
#define LONGITUDE "0.000"
```

### Additional options

Check out the [Wiki](https://github.com/VIPnytt/Frekvens/wiki) for more configuration options.

## ⬆️ Upload to the device

Via PlatformIO, there’s an *Upload* option which uploads the firmware, and a *Upload Filesystem Image* option which uploads the Web app. You’ll need to upload both.

### Over-the-Air updates

Configure in [platformio.ini](https://github.com/VIPnytt/Frekvens/blob/main/platformio.ini):

```ini
[env:seeed_xiao_esp32s3]
board = seeed_xiao_esp32s3
upload_protocol = espota ; Update via Wi-Fi
```

> [!NOTE]
> Migration from Frekvens v1 to v2 requires wired flashing.

## 🏠 Smart-home integrations

### Alexa

Control power and brightness via Amazon Alexa, using voice, app, or automation.

### Home Assistant

Home Assistant is deeply integrated and supports full control over power, brightness, modes, extensions, and more.

## 🤖 API

Four API interfaces are supported:

- [MQTT](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-mqtt)
- [RESTful](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-restful)
- [Server-Sent Events](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-server-sent-events)
- [WebSocket](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-websocket)

Refer to the [Wiki](https://github.com/VIPnytt/Frekvens/wiki) for supported endpoints and use cases.

## ❓ FAQ

### How do I change Wi-Fi network?

Hold any button during startup to activate the Wi-Fi hotspot. Connect to it with your phone or computer, and a configuration portal will open. From there, you can configure new network credentials. Multiple networks can be saved.

## 🚧 Troubleshooting

### Build fails

If the build fails due to memory limits, try disabling features you don’t plan to use in the [`.env`](https://github.com/VIPnytt/Frekvens/blob/main/.env). The [OTA](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-ota) extension is by far the largest one, and therefore a good first candidate to remove in order to free up memory for other stuff.

### Where is the web UI?

First, ensure that the Web app has been uploaded to the device using the *Upload Filesystem Image* option via PlatformIO. Then enter `frekvens.local` or your custom `HOSTNAME.local` in the browser. If this isn’t working, try entering the IP address logged to the terminal at startup.

### Unexpected Reboots

If the device is rebooting unexpectedly, this can be a symptom of a software or hardware issue. The ESP32’s USB port is suitable for flashing and testing, but it is not designed to supply the high current levels that a display might draw during continuous operation.

If terminal logs include error messages, please [report an issue](https://github.com/VIPnytt/Frekvens/issues).
