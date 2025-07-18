# 💡 Frekvens

Frekvens is an ESP32-based mod designed specifically for [IKEA Frekvens](https://github.com/VIPnytt/Frekvens/wiki/Frekvens) and [IKEA Obegränsad](https://github.com/VIPnytt/Frekvens/wiki/Obegransad) devices.

Both devices use the [SCT2024](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf) LED driver, and the firmware is built with flexibility in mind. Thanks to its modular design, it can be easily adapted for custom or third-party devices, including those with different display resolutions or configurations, with only minor adjustments.

## ✨ Features

Featuring replicas of all the same [modes](https://github.com/VIPnytt/Frekvens/wiki/Modes), but incluses everything from new animations and weather services to user-interactive activities and smart-home [extensions](https://github.com/VIPnytt/Frekvens/wiki/Extensions).

The buttons can do more and the [mic](https://github.com/VIPnytt/Frekvens/wiki/Microphone) still syncs with the music. New capabilities have also been added, like [IR](https://github.com/VIPnytt/Frekvens/wiki/Infrared) remote-control support, a feature [IKEA Frekvens](https://github.com/VIPnytt/Frekvens/wiki/Frekvens) was designed for, but never shipped with.

### 📋 Quick summary

- Web app
- 46 [modes](https://github.com/VIPnytt/Frekvens/wiki/Modes)
  - Animations
  - Clocks
  - Interactives
  - Scroller
  - Streaming
  - Weather
- 15 [extensions](https://github.com/VIPnytt/Frekvens/wiki/Extensions)
  - Smart-home integrations
  - Notifications
  - Accessories
- 7 [fonts](https://github.com/VIPnytt/Frekvens/wiki/Fonts)
- 3 API protocols
- [Documentation](https://github.com/VIPnytt/Frekvens/wiki)

## 🔌 Hardware

### 🧠 ESP32

The general recommendation is to get a board with an `ESP32-S`-series chip, at the time of writing (2025) the latest model is an `ESP32-S3`. Any board will do but a compact board like eg. [this](https://www.adafruit.com/product/5426) or [this](https://www.seeedstudio.com/XIAO-ESP32S3-p-5627.html) is enough in most cases. Both of these have 8 MB flash storage which might come in handy if there's ever a need to troubleshoot something in *debug mode*.

For newcomers, it can be a bit challanging to derermine which wire needs to be connected to what pin, but since there's litterally hundreds of ESP32 different boards on the market, there won't be a single set to rule them all. Therefore the documentation will always points out what kind of pin should be used, so that it's easy to look up compatible pins on any board. If in doubt, just [ask](https://github.com/VIPnytt/Frekvens/discussions).

### ➕ Accessories

If desired, extra hardware can be added:

- [IR](https://github.com/VIPnytt/Frekvens/wiki/Infrared)-receiver
- [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Microphone)
- [RTC](https://github.com/VIPnytt/Frekvens/wiki/RTC)-module

**Note:** *[IKEA Frekvens](https://github.com/VIPnytt/Frekvens/wiki/Frekvens) already has a built-in microphone.*

### 🧵 Wiring

Both [IKEA Frekvens](https://github.com/VIPnytt/Frekvens/wiki/Frekvens) and [IKEA Obegränsad](https://github.com/VIPnytt/Frekvens/wiki/Obegransad) have dedicated setup guides explaining everything in detail, but essentially everything you'll have to do is wiring up power, ground, SPI, button and optional accessories to the designated pads.

- [IKEA Frekvens](https://github.com/VIPnytt/Frekvens/wiki/Frekvens#-getting-started)
  - Remove the `U2` chip.
  - The microphone can be wired in from either `U3` pin 7 or `U2` pin 11.
- [IKEA Obegränsad](https://github.com/VIPnytt/Frekvens/wiki/Obegransad#getting-started)
  - Remove the `U1` chip.

**Note:** Both of these devices requires an logic level shifter as ESP32 uses 3.3 V logic.

## 💻 IDE

Feel free to use [any IDE](https://docs.platformio.org/en/latest/integration/ide/index.html) of choice, as long as it supports [PlatformIO](https://platformio.org). For starters, [VS Code](https://code.visualstudio.com) is a good cross-platform candidate, that's also free to use.

Open the project by selecting *clone Git repository* from inside your IDE, then provide this URL: `https://github.com/vipnytt/frekvens.git`.

For those who prefer other alternatives, there's multiple download options at the top of the [project page](https://github.com/vipnytt/frekvens), just hit the green *Code* button.

## 🏗️ PlatformIO

You'll need to select your specific board model to be able to compile a working build. If your board isn't added to the [platformio.ini](tree/main/.env) config already, look it up in the [Boards](https://docs.platformio.org/en/stable/boards/index.html#espressif-32) section over at PlatformIO. Feel free to submit an PR for your board. This way test builds will run for your board too whenever changes are introduced.

[platformio.ini](tree/main/platformio.ini) example:

```ini
[env:seeed_xiao_esp32s3]
board = seeed_xiao_esp32s3
```

## 🔧 Configuration

Almost all config options are optional, including most of the pins.

There's different config files, depending on the scope of the variables. Some are shared between different parts of the project, and is therefore strategically placed in the [.env](tree/main/.env) file in the root directory. Other parts which are firmware specific, should be placed in the [firmware/include/config/secrets.h](tree/main/firmware/include/config/secrets.h) file.

While the [.env](tree/main/.env) file by default contains a full list of *shared* variables, the firmware equalent [secrets.h](tree/main/firmware/include/config/secrets.h) file is empty. This is due to the extensive list of optional variables, where many needs some sort of explanation for what they do. Therefore it's best to check out the [Wiki](https://github.com/VIPnytt/Frekvens/wiki) for options instead.

### 📝 Template

Depending on device type, the [.env](tree/main/.env) should contain one of these two:

```ini
# IKEA Frekvens
ENV_FREKVENS=''
```

```ini
# IKEA Obegränsad
ENV_OBEGRANSAD=''
```

In the [secrets.h](tree/main/firmware/include/config/secrets.h), define where everything is connected:

```h
#pragma once

// GPIO pins
#define PIN_SCLK 1
#define PIN_MOSI 2
#define PIN_CS 3
#define PIN_EN 4
#define PIN_SW2 5
```

```h
// IKEA Frekvens only
#define PIN_SW1 6
#define PIN_MIC 7
```

### 🌍 Time zone

Set the time zone in the [.env](tree/main/.env) config file. It's expected to be in *IANA* format, eg. `America/New_York`, `Asia/Shanghai` or `Europe/Istanbul`. [Lookup](https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv) your time zone if needed.

```ini
# Time Zone
TIME_ZONE_IANA='Etc/Universal'
```

For more info, check out the [Time zone](https://github.com/VIPnytt/Frekvens/wiki/Services#-time-zone) section in the Wiki.

### 📶 Wi-Fi

Wi-Fi credentials can both be configured via the [secrets.h](tree/main/firmware/include/config/secrets.h) config file, and at any time using the *web app* user-interface. Multiple Wi-Fi networks is supported.

If no Wi-Fi credentials is provided on first boot, the device will set up an Wi-Fi hotspot for configuration. This hotspot feature can also be activated later by holding any physical button on the device during startup. For security reasons, it won't activate if the Wi-Fi signal is lost or out of reach.

```h
// Wi-Fi (optional)
#define WIFI_SSID "name"
#define WIFI_KEY "password"
```

For more info, check out the [Wi-Fi](https://github.com/VIPnytt/Frekvens/wiki/Services#-wi-fi) section in the Wiki.

### ☀️ Weather

There's multiple weather providers available, common for them all is that they use coordinates to derermine the closest location. Three decimals is usally sufficient.

[secrets.h](tree/main/firmware/include/config/secrets.h) example:

```h
// Weather (optional)
#define LATITUDE "0.000"  // coordinate
#define LONGITUDE "0.000" // coordinate
```

For more info, check out the [Weather](https://github.com/VIPnytt/Frekvens/wiki/Services#-weather) section in the Wiki.

### 🎞️ Frame rate

The achievable frame rate depends on the SPI speed. Real-world factors like wiring layout, connection quality, and cable length can affect reliability and may require tuning for optimal performance.

[secrets.h](tree/main/firmware/include/config/secrets.h) examples:

```h
#define FRAME_RATE 60 // fps
#define SPI_FREQUENCY 16000000 // Hz
```

```h
#define FRAME_RATE 50 // fps
#define SPI_FREQUENCY 8888888 // Hz
```

```h
#define FRAME_RATE 40 // fps
#define SPI_FREQUENCY 6666666 // Hz
```

For more info, check out the [Display](https://github.com/VIPnytt/Frekvens/wiki/Services#-display) section in the Wiki.

## ⬆️ Upload

When the configuration is done, it's time to build and upload. There's *[documentation](https://docs.platformio.org/en/latest/integration/ide/index.html)* for most IDEs, but for *[VS Code](https://docs.platformio.org/en/latest/integration/ide/vscode.html)* you'll essentially find the all the options in the *PlatformIO* menu on left side.

Doing a *regular* "Upload" will upload the `firmware.bin` file only. The `webapp` *web app* is located in `spiffs.bin` which needs to be uploaded separately using *"Upload Filesystem Image"*.

### 📡 Over the Air updates

By extending your boards configuration with `env:upload_ota`, the upload will happen over Wi-Fi instead of USB.

[platformio.ini](tree/main/platformio.ini) example:

```ini
extends = env:upload_ota
```

Make sure to set the correct *host*, or *IP address* in the `upload_port` option. If [password protection](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-ota) has been enabled using `OTA_KEY` or `OTA_KEY_HASH`, make sure also the password is specified in the `--auth` option of `upload_flags`.

[platformio.ini](tree/main/platformio.ini) example:

```ini
upload_protocol = espota
upload_port = example.local
;upload_flags = --auth=password
```

While OTA updates directly from the IDE is the easiest, there's also a user-interface for manual upload of `firmware.bin` and `spiffs.bin` files in the *web app*.

For more info, check out the [OTA](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-ota) section in the Wiki.

### 🔄 Migration from similar projects

While wired upload is recommended the very first time, existing devices with alternative firmware are in some cases able to migrate OTA via Wi-Fi.

Follow the same instructions as for [OTA](#-over-the-air-updates) directly above, but swap out `upload_ota` with `upload_ota_migration` instead. This way there will be presented different upload options, wether your existing device uses `espota`, `ElegantOTA`/`AsyncElegantOTA` or `WiFiManager`.

Always upload the regular `firmware` first, if any of the two uploads fails, it just means you'll have to flash via USB as the partition scheme needs to be altered slightly to fit everything.

## 🏠 Smart-home integrations

In theory, any custom smart-home system can be integrated using [RESTful](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-restful), [MQTT](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-mqtt) or [WebSocket](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-websocket). However, when it comes to ready-made integrations that are easy to set up for end-users, these are currently two options:

### 🎙️ Alexa

*[Amazon Alexa](https://www.amazon.com/b?node=9818047011)* users are able to switch on/off their device, as well as control the brightness, via app, voice or automations.

For more info, check out the [Alexa](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-alexa) section in the Wiki.

### 🤖 Home Assistant

Most features are supported in *[Home Assistant](https://www.home-assistant.io)*, including switching on/off, controlling brightness, mode, settings, extensions and modes. Use the *Home Assistant* app, voice or even automations to control them all.

For more info, check out the [Home Assistant](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-home-assistant) section in the Wiki.

## 🤝 API

There's three different API interfaces available, each suited for different applications, but they're all fully equipped, meaning any interface can do any command.

- [RESTful](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-restful)
- [MQTT](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-mqtt)
- [WebSocket](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-websocket)

In addition to what's already available via the *web app* user-interface, there's even more features hiding under the surface, including a lot of meta data.

Please visit the [Wiki](https://github.com/VIPnytt/Frekvens/wiki) to learn more info about what's supported in each [service](https://github.com/VIPnytt/Frekvens/wiki/Services), [extension](https://github.com/VIPnytt/Frekvens/wiki/Extensions) or [mode](https://github.com/VIPnytt/Frekvens/wiki/Modes).

## ❓ FAQ

### Where is the *web app* user-interface?

Make sure to upload the *filesystem* image too, as described in the [upload](#️-upload) section.

Both the *domain* and *IP address* is logged to the terminal during startup, open either of them in the web browser. *It's also possible to look up the these values using various "network discovery" apps.*

### How can I change Wi-Fi network?

If the device is no longer connected to any Wi-Fi network, disconnect the power, press and hold any physical button on the device, and then re-connect power. Release the button about 5-10 seconds after the display lits up. A Wi-Fi hotspot network should apear, connect to it and open the web browser to input the new Wi-Fi credentials.

If you're still able to access the device via your old Wi-Fi, the process is much simpler, just connect to the new Wi-Fi network via the *web app* user-interface instead.

### How about the ESP8266, is supported?

No, it's considered a legacy platform and lacks the performance needed to run this project smoothly.

## 🚧 Troubleshooting

### The display is flickering

If you're seeing *"snow"*, this is usually an indication of EMI caused by bad wiring and/or connections. Keep it short and avoid pluggable connectors, especially jumper-wires and breadboards. Noise from some power supplies can also cause trouble, which in most cases can be prevented by installing capacitors. Make sure to follow the guidelines for your specific device, [IKEA Frekvens](https://github.com/VIPnytt/Frekvens/wiki/Frekvens) or [IKEA Obegränsad](https://github.com/VIPnytt/Frekvens/wiki/Obegransad).

If the display otherwise is looking normal, try increasing the [frame rate](https://github.com/VIPnytt/Frekvens/wiki/Services#-display).

### The device reboots randomly

The [IKEA Obegränsad](https://github.com/VIPnytt/Frekvens/wiki/Obegransad) draws approximately 2.0 A @ 5 V when idling and fully illuminated at maximum brightness. Ensure that the USB power supply can deliver at least 10 W of continuous power.

If you're got error messages in the terminal, feel free to create a [issue](https://github.com/VIPnytt/Frekvens/issues) about it.
