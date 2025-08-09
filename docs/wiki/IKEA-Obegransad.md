# 💡 IKEA Obegränsad LED wall lamp

> Article number 005.262.48

## 📈 Schematics

### LED panels schema

```text
           ┌──────────────────────── +5 V DC
           │   ┌──────────────────── SPI CS
           │   │   ┌──────────────── SPI SCLK
           │   │   │   ┌──────────── SPI MOSI
           │   │   │   │   ┌──────── Enable
           │   │   │   │   │   ┌──── 0 V DC
    ┌──────┼───┼───┼───┼───┼───┼──┐
    │     VCC CLA CLK DI  EN  GND │
    │ SW                          │
    │ SW1   U1 ┌┴─┴─┴─┴─┴─┴─┴─┴┐  │
┌───┼ DC-      └┬─┬─┬─┬─┬─┬─┬─┬┘  │
│ ┌─┼ DC+                   └─────┼─ Button
│ │ │     VCC CLA CLK DO  EN  GND │
│ │ └──────────────────┼──────────┘
│ │                    └──────────── SPI MISO
│ │
│ └───────────────────────────────── +5 V DC
└─────────────────────────────────── 0 V DC
```

> There's four daisy-chained panels, here shown as one combined unit.

### Button schema

```text
┌─────┐
│  SW ├─ Button
│ SW1 ├─ 0 V DC
└─────┘
```

### USB cable schema

```text
───────┐
White ─┼─ +5 V DC
Black ─┼─ 0 V DC
───────┘
```

### ESP32 schema

```text
┌────────────────┐
│            VIN ├─ +5 V DC
│            3V3 ├─ +3.3 V DC
│            GND ├─ 0 V DC
│                │
│       SPI SCLK ├─ SPI SCLK
│       SPI MISO ├─ SPI MISO
│       SPI MOSI ├─ SPI MOSI
│                │
│ Digital output ├─ SPI CS
│                │
│     PWM output ├─ Enable
│                │
│  Digital input ├─ Button
└────────────────┘
```

### Logic level shifter schema

```text
   0 V DC ────────┬──────── 0 V DC
+3.3 V DC ────┐   │   ┌──── +5 V DC
           ┌──┴───┴───┴──┐
           │ VCC GND VCC │
 SPI SCLK ─┤             ├─ SPI SCLK
 SPI MISO ─┤             ├─ SPI MISO
 SPI MOSI ─┤             ├─ SPI MOSI
   SPI CS ─┤             ├─ SPI CS
   Enable ─┤             ├─ Enable
           │             │
           └─────────────┘
```

## 🚀 Getting started

### Opening the back panel

Several methods for opening the device have been shared online. Among them, the most *consistently reliable* and *non-destructive* approach appears to involve the careful use of a drill.

By using a sharp 3 mm drill bit and drillilg *slowly*, the aluminium rivets will mostly form a spiral and leave minimal amounts of swarf. Stop drilling once the top edge of the rivets breaks. Once they're all gone, remove the back panel and use a 2 mm drill bit to remove the last fragments stuck in the holes, preferably with the help of small pliers.

For those who desire, it's possible to use 2 mm rivets to close the device afterwards.

> There isn't really risk of hitting anything inside when drilling, as it's basically just a empty box. Most of the electronic components is covered up, but make sure to clean up any metal fragments before powering on.

### Removing the `U1` chip

The first thing to do is removing the chip labeled `U1`, as this is the core handling both the button input and display output. It's easy to locate as the device consists of 4 panels where the `U1` chip is only present on one of them.

### Wiring the LED panels

Next up is attaching the ESP32, via the [logic level shifter](#%EF%B8%8F-logic-level-shifter). There's two empty pads, one in the top, and the other at the bottom, labeled `IN` and `OUT`. Connect all 6 wires as shown at the top of the schema. When it comes to the bottom pad, the `DO` is optional to connect.

> The LED panels are rotated 180° compared to the device's natural orientation.

### Connecting the button

There's different options based on skill level and time investment:

- *Best practice:*
  - Disconnect both `SW` and `SW1`.
  - Connect one end to ESP32 and the other end to `GND`.
- *Minimalistic:*
  - Connect the now empty `U1` pad 7 to the ESP32.
  - Leave both `SW` and `SW1` connected.
- *Beginner friendly:*
  - Disconnect `SW` and then re-connect it to ESP32 instead.
  - Leave `SW1` connected.

## ↔️ Logic level shifter

The [ESP32](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf) operates at 3.3 V logic, while the [SCT2024](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf) operates at 5 V logic. Therefore, a bidirectional logic level shifter compatible with SPI, such as the [TXB0104](https://www.adafruit.com/product/1875) or [TXB0108](https://www.adafruit.com/product/395), is required to ensure reliable and safe communication between the devices.

- The [ESP32](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)'s logic *HIGH* level is below [SCT2024](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf)'s minimum voltage requirement:
  - SPI SCLK *`CLK`*
  - SPI MOSI *`DI`*
  - SPI CS *`CLA`*
  - Enable *`EN`*
- The [SCT2024](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf)'s logic *HIGH* level is above [ESP32](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)'s absolute maximum voltage rating on its GPIO pins:
  - SPI MISO *`DO`*
  - Enable *`EN`*

> While there are reports suggesting that these devices can work together without level shifting, this is strongly discouraged. The [SCT2024](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf) is not guaranteed to operate reliably at 3.3 V logic levels, and exceeding the [ESP32](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)’s voltage ratings may result in unpredictable behavior or permanent damage to the microcontroller.

## 🛠️ Hardware considerations

The *IKEA Obegränsad* hardware is sub-optimally designed, but fortunately for those who desire it's easy and straight forward to do something about it.

Prioritize the wiring, but feel free to skip the capacitors if there isn't any to spare.

### Wiring

The power supply (USB) should be connected to every `DC+` and `DC-` pad. This will help reduce the current travelling through the traces on the PCB, reduce voltage losses, and ultimately frequency noise.

**Suggestions:**

- Connect all four `DC+` terminals with wires.
- Connect all four `DC-` terminals with wires.

**Tip:** Keep the wires as short as possible.

### Capacitors

For dimming of the display, the [SCT2024](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf) datasheet suggests adding an additional *"4.7 µF or more"* depending on the LED load current (~7 mA/output), for every chip. With four chips, that's a minimum of 18.8 µF per LED panel. The same datasheet also recommends adding *"greater than 10 µF"* besides the LEDs, bringing the total up to 28.8 µF each. With four LED panels, that's a combined minimum of 115.2 µF.

The capacitors will help prevent timing issues, as well as minimize noise from the USB power supply. Ideally they should be placed as close to the load as possible.

**Suggestion:**

- Add at least 28.8 µF to each of the four panels.

**Tip:** The four `DC+` and `DC-` pads are good positions for soldering capacitors. Other positions are the two `VCC` and `GND` pads on the top and bottom. Also the `J3` JTAG pads can be retrofitted if desired, `J3` pin 1 (square) is tied to `VCC` while `J3` pin 4 (rectangle) is tied to `GND`. For those who prefer SMDs, theres two labeled `C2` and `C7` on each panel, except the first LED panel where `C2` is occupied as it was paired with the `U1` chip which got disassembled.

## 🔧 Configuration

| Obegränsad | Via                 | ESP32          | Function | Constant   |
| ---------- | ------------------- | -------------- | -------- | ---------- |
| `CLK`      | Logic level shifter | SPI SCLK       | SPI SCLK | `PIN_SCLK` |
| `DO`       | Logic level shifter | SPI MISO       | SPI MISO | `PIN_MISO` |
| `DI`       | Logic level shifter | SPI MOSI       | SPI MOSI | `PIN_MOSI` |
| `CLA`      | Logic level shifter | Digital output | SPI CS   | `PIN_CS`   |
| `EN`       | Logic level shifter | PWM output     | Enable   | `PIN_EN`   |
| `SW`       |                     | Digital input  | Button   | `PIN_SW2`  |

### Power and ground

- `DC+` and `DC-` is intended to supply high current to the LEDs.
- `VCC` and `GND` is intended as outputs for low corrent components handling the logic.
- Both are tied together internally on the PCB.

> Do not use the ESP32's USB port while the *IKEA Obegränsad's* USB-cable is connected to a power source!

### SPI SCLK

[Logic level shifter](#%EF%B8%8F-logic-level-shifter) required.

Use any *SPI `SCLK`* pin.

> If the board has two sets of SPI pins, choose any of them, but be consistent and always use either `HSPI` or `VSPI`.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_SCLK 1 // GPIO #
```

### SPI MISO

Optional to connect, but if so, an [logic level shifter](#%EF%B8%8F-logic-level-shifter) is required.

Use any *SPI `MISO`* pin.

> If the board has two sets of SPI pins, choose any of them, but be consistent and always use either `HSPI` or `VSPI`.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_MISO 2 // GPIO #
```

### SPI MOSI

[Logic level shifter](#%EF%B8%8F-logic-level-shifter) required.

Use any *SPI `MOSI`* pin.

> If the board has two sets of SPI pins, choose any of them, but be consistent and always use either `HSPI` or `VSPI`.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_MOSI 3 // GPIO #
```

### SPI CS

[Logic level shifter](#%EF%B8%8F-logic-level-shifter) required.

Use any *digital output* pin. *First generation ESP32 boards may have specialized pins (`CS`/`SS`) that are preferable to other pins.*

> Avoid strapping pins as this pin is pulled *LOW* using a resistor.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_CS 4 // GPIO #
```

### Enable

[Logic level shifter](#%EF%B8%8F-logic-level-shifter) required.

Use any *PWM output* pin.

> Avoid strapping pins as this pin is pulled *HIGH* using a resistor.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_EN 5 // GPIO #
```

### Button

Optional to connect.

Use any *digital input* pin, preferably one that is also RTC-capable, to allow deep sleep wake-up functionality.

> Avoid strapping pins as this pin is pulled *LOW* when pressed.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_SW2 6 // GPIO #
```

> The other end of the button must be connected to ground. By default, it's connected to `SW1` which is tied to `GND` via `DC-`.

## 📝 Template

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
# Device type
ENV_OBEGRANSAD=''

# Custom device name (optional)
NAME='Obegränsad'

# Time zone (optional)
TIME_ZONE_IANA='Etc/Universal'
```

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#pragma once

// GPIO pins
#define PIN_SCLK 1
#define PIN_MOSI 2
#define PIN_CS 3
#define PIN_EN 4
#define PIN_SW2 5

// Wi-Fi credentials (optional)
#define WIFI_SSID "name"
#define WIFI_KEY "secret"

// Weather location (optional)
#define LATITUDE "0.000"
#define LONGITUDE "0.000"
```
