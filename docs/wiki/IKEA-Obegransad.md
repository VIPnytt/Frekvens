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

The button can be wired in several ways, depending on how much modification is preferred. The most straightforward approach is to connect it directly between the ESP32 and `GND`. This method is recommended because it makes the wiring easy to follow and keeps the setup visually clear.

For those who prefer to reuse existing connections, the wire connected to `SW` can be re-routed to the ESP32 instead. As an alternative with minimal changes, it is also possible to connect the empty `U1` pad 7 directly to the ESP32.

> On the LED panel, `SW1` is already tied to `GND`.

## ↔️ Logic level shifter

For safe and reliable communication between the ESP32 and the LED panels, a suitable logic level shifter is required.

The ESP32’s 3.3 V signals are too weak for the [SCT2024](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf) to reliably register. At the same time, the [SCT2024](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf) outputs signals at 5 V and uses pull-ups on its inputs — both of which can feed unsafe voltages back into the ESP32. To protect the microcontroller and ensure consistent communication, *all signal lines must go through a level shifter*.

> Some users have reported success without level shifting, but this is outside the specifications. Skipping it can lead to misread signals, unstable behavior, or even permanent damage to the ESP32.

## 🛠️ Hardware considerations

The *IKEA Obegränsad* hardware is sub-optimally designed, but fortunately for those who desire it's easy and straight forward to do something about it.

### Wiring

The USB cable powers the first LED panel by connecting to its `DC+` and `DC-` pads. Power is then passed along to the other panels through internal traces and the secondary bus interface.

For best performance, however, the power supply should also be connected directly to the `DC+` and `DC-` pads on all four panels. This ensures shorter current paths, more even power delivery, and reduced noise.

### Capacitors

The original `U1` microcontroller lacked dimming capabilities, but the ESP32 enables full PWM dimming. Because PWM involves rapid switching, it can stress the power supply and introduce noise. Decoupling capacitors are recommended to smooth out these fluctuations, improving stability and preventing timing issues.

The [SCT2024 datasheet](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf) suggests a total of ≈≥85 µF, placed as close as possible to the [SCT2024](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf) chips and LEDs. Optimal performance is achieved by distributing capacitance across multiple solder points, rather than concentrating it in a single large capacitor.

**Easily accessible soldering points:**

- 4x `DC+` / `DC-` pads — one at each panel
- 2x `VCC` / `GND` pads — top and bottom
- 4x `J3` JTAG pads — pin 1 (square, `VCC`) and pin 4 (round, `GND`)
- 3x `C2` SMD pads — one at each panel, the fourth is already populated
- 4x `C7` SMD pads — one at each panel

> Capacitors are optional — consider adding them if you already have some available, or if you notice flicker or instability.

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

- `DC+` / `DC-` is intended to supply high current to the LEDs.
- `VCC` / `GND` is intended as outputs for low current components handling the logic.
- Both are tied together internally on the PCB.

> Do not use the ESP32's USB port while the *IKEA Obegränsad's* USB-cable is connected to a power source!

### SPI SCLK

[Logic level shifter](#%EF%B8%8F-logic-level-shifter) required.

Any *SPI `SCLK`* pin can be used.

> The use of either the `HSPI` or `VSPI` bus is required for consistency on boards with two SPI interfaces.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_SCLK 1 // GPIO #
```

### SPI MISO

Optional to connect, [logic level shifter](#%EF%B8%8F-logic-level-shifter) required.

Any *SPI `MISO`* pin can be used.

> The use of either the `HSPI` or `VSPI` bus is required for consistency on boards with two SPI interfaces.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_MISO 2 // GPIO #
```

### SPI MOSI

[Logic level shifter](#%EF%B8%8F-logic-level-shifter) required.

Any *SPI `MOSI`* pin can be used.

> The use of either the `HSPI` or `VSPI` bus is required for consistency on boards with two SPI interfaces.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_MOSI 3 // GPIO #
```

### SPI CS

[Logic level shifter](#%EF%B8%8F-logic-level-shifter) required.

Any *digital output* pin can be used.

> Avoid strapping pins as this pin is pulled *LOW* using a resistor. On ESP32 (LX6-based, original series) boards, it is recommended to use specialized pins, such as `CS` (often labeled `SS` on older boards).

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_CS 4 // GPIO #
```

### Enable

[Logic level shifter](#%EF%B8%8F-logic-level-shifter) required.

Any *PWM output* pin can be used.

> Avoid strapping pins as this pin is pulled *HIGH* using a resistor.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_EN 5 // GPIO #
```

### Button

Optional to connect.

Any *digital input* pin can be used, but those that are also RTC-capable are preferred.

> Avoid strapping pins as this pin is pulled *LOW* when pressed.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_SW2 6 // GPIO #
```

## 📝 Template

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
# Device type
ENV_OBEGRANSAD=''

# Custom device name (optional)
NAME='Obegränsad'
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
