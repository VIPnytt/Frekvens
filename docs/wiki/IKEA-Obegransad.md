# 💡 IKEA Obegränsad LED wall lamp

Article number: `005.262.48`

## 📌 Schematics

### LED panels schema

```text
                ┌──────────────────────── +5 V DC
                │   ┌──────────────────── SPI CS
                │   │   ┌──────────────── SPI SCLK
                │   │   │   ┌──────────── SPI MOSI
                │   │   │   │   ┌──────── OE
                │   │   │   │   │   ┌──── 0 V DC
         ┌──────┼───┼───┼───┼───┼───┼──┐
         │     VCC CLA CLK DI  EN  GND │
 Button ─┼ SW                          │
         │ SW1   U1 ┌┴─┴─┴─┴─┴─┴─┴─┴┐  │
 0 V DC ─┼ DC-      └┬─┬─┬─┬─┬─┬─┬─┬┘  │
+5 V DC ─┼ DC+                   └─────┼─ Button
         │     VCC CLA CLK DO  EN  GND │
         └─────────────────────────────┘
```

> [!NOTE]
> There’s four daisy-chained panels, here shown as one combined unit.

### Button schema

```text
┌─────┐
│  SW ├─ Button
│ SW1 ├─ 0 V DC
└─────┘
```

### USB cable schema

```text
──────┐
White ┼─ +5 V DC
Black ┼─ 0 V DC
──────┘
```

### ESP32 schema

```text
┌────────────────┐
│            VIN ├─ +5 V DC
│            3V3 ├─ +3.3 V DC
│            GND ├─ 0 V DC
│                │
│           SCLK ├─ SPI SCLK
│           MOSI ├─ SPI MOSI
│                │
│ Digital output ├─ SPI CS
│ Digital output ├─ OE
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
   SPI CS ─┤     ──►     ├─ SPI CS
 SPI SCLK ─┤     ──►     ├─ SPI SCLK
 SPI MOSI ─┤     ──►     ├─ SPI MOSI
       OE ─┤     ──►     ├─ OE
           └─────────────┘
```

## 🚀 Getting started

### Opening the back panel

Several methods for opening the device have been shared online. Among them, the most *consistently reliable* and *non-destructive* approach appears to involve the careful use of a drill.

By using a 3 mm drill bit and drillilg *slowly*, the aluminium rivets will mostly form a spiral and leave minimal amounts of swarf. Stop drilling once the top edge of the rivets breaks. Once they’re all gone, remove the back panel and use a 2 mm drill bit to remove the last fragments stuck in the holes, optionally with the help of small pliers.

For those who desire, it’s possible to use 2 mm rivets to re-seal the device afterwards.

> [!TIP]
> There isn’t really risk of hitting anything inside when drilling, as it’s basically just a empty box. Most of the electronic components is covered up, but make sure to clean up any metal fragments before powering on.

### Removing the `U1` chip

The first thing to do is removing the chip labeled `U1`, as this is the core handling both the button input and display output. It’s easy to locate as the device consists of 4 panels where the `U1` chip is only present on one of them.

### Wiring the LED panels

Next up is attaching the ESP32, via the [logic level shifter](#%EF%B8%8F-logic-level-shifter). There’s two noteworthy locations, one in the top labeled `IN`, and the other at the bottom labeled `OUT`. There’s no difference between input and output, except for `DI` and `DO`.

> [!NOTE]
> The LED panels are rotated 180° compared to the device’s natural orientation.

### Connecting the button

The button can be wired in several ways, depending on the desired level of modification. The simplest and most visually clear method is to connect it directly between the ESP32 and `GND`. This keeps the wiring straightforward and self-contained, reducing the chance of confusion from the PCB’s existing circuitry.

If reusing existing connections is preferred, the wire on the `SW` pad can either be re-routed to the ESP32 or simply spliced by adding a second wire to the same pad. For those comfortable with fine-pitch soldering, the newly desoldered `U1` pad 7 — provides another solder point with the same electrical result.

> [!NOTE]
> On the LED panel, `SW1` is internally tied to `GND`.

## ↔️ Logic level shifter

For safe and reliable communication between the ESP32 and the LED panels, a suitable logic level shifter is required.

The ESP32’s 3.3 V signals are too weak for the [SCT2024](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf) to reliably register. At the same time, the [SCT2024](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf) outputs signals at 5 V and uses pull-ups on its inputs — both of which can feed unsafe voltages back into the ESP32. To protect the microcontroller and ensure consistent communication, *all signal lines must go through a level shifter*.

> [!WARNING]
> Some users have reported success without level shifting, but this is outside the specifications. Skipping it can lead to misread signals, unstable behavior, or even permanent damage to the ESP32.

## 🛠️ Hardware considerations

The *IKEA Obegränsad* hardware is sub-optimally designed, but fortunately for those who desire it’s easy and straight forward to do something about it.

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

> [!NOTE]
> Capacitors are optional — consider adding them if you already have some available, or if you notice flicker or instability.

## 🔧 Configuration

| Label | Type           | Constant   |
| ----- | -------------- | ---------- |
| `CLA` | Digital output | `PIN_CS`   |
| `CLK` | SPI SCLK       | `PIN_SCLK` |
| `DI`  | SPI MOSI       | `PIN_MOSI` |
| `EN`  | Digital output | `PIN_OE`   |
| `SW`  | Digital input  | `PIN_SW2`  |

### Power and ground

Supplies power to both logic and LEDs.

- `DC+` / `DC-` is intended to supply high current to the LEDs.
- `VCC` / `GND` is intended as outputs for low current components handling the logic.
- Both are tied together internally on the PCB.

The *IKEA Obegränsad* can draw up to 2.0 A at 5 V (10 W) under full load. Use a USB power supply rated for at least this continuous output.

> [!CAUTION]
> Do not use the ESP32’s USB port while the *IKEA Obegränsad’s* USB-cable is connected to a power source!

### SPI CS

Chip Select for the LED drivers.

Any digital output pin can be used.

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define PIN_CS 1 // CLA
```

> [!IMPORTANT]
> [Logic level shifter](#%EF%B8%8F-logic-level-shifter) required.

> [!WARNING]
> Avoid strapping pins as this pin is pulled *LOW* with 25 kΩ resistance. On ESP32 classic it is recommended to use specialized pins, such as `CS` (sometimes labeled `SS` on older boards).

### SPI SCLK

Serial clock for SPI communication.

Any SPI `SCLK` pin can be used.

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define PIN_SCLK 2 // CLK
```

> [!IMPORTANT]
> [Logic level shifter](#%EF%B8%8F-logic-level-shifter) required.

### SPI MOSI

Master-out data line for SPI.

Any SPI `MOSI` pin can be used.

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define PIN_MOSI 3 // DI
```

> [!IMPORTANT]
> [Logic level shifter](#%EF%B8%8F-logic-level-shifter) required.

### Output Enable

Enables or disables LED output.

Any digital output pin can be used.

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define PIN_OE 4 // EN
```

> [!IMPORTANT]
> [Logic level shifter](#%EF%B8%8F-logic-level-shifter) required.

> [!WARNING]
> Avoid strapping pins as this pin is pulled *HIGH* with 25 kΩ resistance.

### Button

Button input for user interaction.

Optional to connect. Use an RTC-capable digital input pin for best compatibility.

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define PIN_SW2 5 // SW
```

> [!IMPORTANT]
> Avoid strapping pins as this pin is pulled *LOW* when pressed.

## 📝 Template

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
# Device type
IKEA_OBEGRANSAD='true'

# Names
NAME='Obegränsad'
HOSTNAME='obegransad'
```

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#pragma once

// GPIO pins
#define PIN_CS 1   // CLA
#define PIN_SCLK 2 // CLK
#define PIN_MOSI 3 // DI
#define PIN_OE 4   // EN
#define PIN_SW2 5  // SW

// Wi-Fi credentials
#define WIFI_SSID "name"
#define WIFI_KEY "secret"

// Weather location (optional)
#define LATITUDE "0.000"
#define LONGITUDE "0.000"
```

## 🔗 Resources

A collection of external links for deeper exploration — including teardowns, mods, datasheets, and community projects — provided for reference only and with no formal connection to this project.

- [GitHub: a-johanson/rusty-obegraensad](https://github.com/a-johanson/rusty-obegraensad/blob/master/README.md)
- [GitHub: atesgoral/obegraensad-hack](https://github.com/atesgoral/obegraensad-hack/blob/main/README.md)
- [GitHub: Drewbi/oberled](https://github.com/Drewbi/oberled/blob/main/README.md)
- [GitHub: MakeMagazinDE/Obegraensad](https://github.com/MakeMagazinDE/Obegraensad/blob/main/README.md)
- [GitHub: ph1p/ikea-led-obegraensad](https://github.com/ph1p/ikea-led-obegraensad/blob/main/README.md)
- [GitHub: unframework/obehack-202301](https://github.com/unframework/obehack-202301/blob/main/README.md)
- [Hackaday: IKEA OBEGRÄNSAD LED wall lamp + ESP32](https://hackaday.io/project/188987-ikea-obegrnsad-led-wall-lamp-esp32)
- [Hackaday: Hacking The IKEA OBEGRÄNSAD LED Wall Lamp](https://hackaday.com/2023/05/25/hacking-the-ikea-obegransad-led-wall-lamp/)
- [Heise magazine: IKEA-Matrix gehackt](https://www.heise.de/select/make/2023/6/2317908271368569840)
- [IKEA: OBEGRÄNSAD PR-boekje](https://www.ikea.com/nl/nl/files/pdf/3b/ff/3bff5e70/obegransad-pr-boekje-incl-pricing.pdf)
- [Pixelkönig Blog: X-Clock (IKEA OBEGRÄNSAD Hack)](http://blog.digital-image.de/2023/05/31/x-clock/)
- [StarChips: SCT2024 DataSheet](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf)
