# 💡 IKEA Frekvens LED multi-use light

Article number: `504.203.53`

## 📌 Schematics

### LED panel schema

```text
             ┌────────────────────── SPI CS
             │   ┌────────────────── SPI SCLK
             │   │   ┌────────────── SPI MOSI
             │   │   │   ┌────────── OE
┌────────────┼───┼───┼───┼────────┐
│ ┌──────┴───┴───┴───┴───┴───┴──┐ │
│ │     GND LAK CLK DA  EN  VCC │ │
│ │                             │ │
│ │ SW1      U2 ┌┴─┴─┴─┴─┴─┴─┴┐ │ │
│ │ SW2  89F112 └┬─┬─┬─┬─┬─┬─┬┘ │ │
│ │ COM                │        │ │
│ │               ┌─┐  │        │ │
│ │            U3 ┤ ├  │        │ │
│ │         LM358 ┤ ├──┴────────┼─┼─ Amplifier
│ ┤ IR            ┤ ├           │ │
│ │               ┤ ├           │ │
│ │ MIC           └─┘           │ │
│ └──┬──────────────────────────┘ │
│ GND LAK CLK DA  EN  VCC DC- DC+ │
└──────────────────────────┼───┼──┘
                           │   └──── +4 V DC
                           └──────── 0 V DC
```

### Buttons schema

```text
┌────────────┐
│ COM SW1 SW │
└──┼───┼───┼─┘
   │   │   └─── Button 2
   │   └─────── Button 1
   └─────────── 0 V DC
```

### Power supply schema

```text
              ┌───────┐
100-240 V AC ─┤ L   + ├─ +4 V DC
     Neutral ─┤ N   - ├─ 0 V DC
              └───────┘
```

### ESP32 schema

```text
┌────────────────┐
│            VIN ├─ +4 V DC
│            3V3 ├─ +3.3 V DC
│            GND ├─ 0 V DC
│                │
│           SCLK ├─ SPI SCLK
│           MOSI ├─ SPI MOSI
│                │
│ Digital output ├─ SPI CS
│ Digital output ├─ OE
│                │
│  Digital input ├─ Button 1
│  Digital input ├─ Button 2
│                │
│   Analog input ├─ Amplifier
└────────────────┘
```

### Logic level shifter schema

```text
   0 V DC ────────┬──────── 0 V DC
+3.3 V DC ────┐   │   ┌──── +4 V DC
           ┌──┴───┴───┴──┐
           │ VCC GND VCC │
   SPI CS ─┤     ──►     ├─ SPI CS
 SPI SCLK ─┤     ──►     ├─ SPI SCLK
 SPI MOSI ─┤     ──►     ├─ SPI MOSI
       OE ─┤     ──►     ├─ OE
           └─────────────┘
```

## 🚀 Getting started

### Opening up

After loosening the four screws and opening the back panel, there’s eight nuts held in place by four plastic clips. Once those and the four screws in the bottom are removed, everything slides out easily.

### Disconnect the buttons

The next step is to disconnect the buttons from the green PCB, `SW1`, `SW2` and `COM`.

### Removing the `U2` chip

This step can be a bit challanging; removing the [89F112](https://lceda.cn/components/89F112_aeaaa99e4cd44677a24b9884cee22ff3) chip labeled `U2` from the green PCB.

> [!TIP]
> If the integrated microphone is not needed, the entire green PCB can be desoldered from the LED panel. This makes the modification easier, while still allowing the board to be re-soldered back on later if desired.

### Wiring the microphone

For the microphone to be functional, connect a wire from the [LM358](https://www.onsemi.com/download/data-sheet/pdf/lm358-d.pdf) amplifier labeled `U3` pin 7 to an *analog input* on the ESP32.

> [!TIP]
> Since the [89F112](https://lceda.cn/components/89F112_aeaaa99e4cd44677a24b9884cee22ff3) chip must be desoldered, it might be easier to connect from `U2` pad 11 instead.

### The Missing IR sensor

The device was originally designed with an IR sensor in mind, but the feature was never implemented in production — leaving the `IR` pad unused.

The hole next to `C10` was reserved for this purpose and can still be used to mount an [IR sensor](https://github.com/VIPnytt/Frekvens/wiki/Infrared-receiver), enabling a capability that was planned in the original design but never realized.

### Wiring the LED panel

Next up is attaching the ESP32, via the [logic level shifter](#%EF%B8%8F-logic-level-shifter). There’s two noteworthy locations, the first one is on top of the green PCB, and the second one is the six unlabeled pads at bottom left. There’s no difference between them except for `DA`, where the labeled one on top is input and the unlabeled at bottom is output.

> [!CAUTION]
> The two left-most unlabeled pads will be blocked by the chassis.

### Connecting the button

The last step is to connect the buttons: `SW` and `SW1` each connect to separate pins on the ESP32, while `COM` connects to `GND`.

> [!TIP]
> On the stock wiring harness, the red power button `SW1` uses the *black* wire, and the yellow mode button `SW` uses the *white* wire. They both share a *red* wire for `COM`.

## ↔️ Logic level shifter

For safe and reliable communication between the ESP32 and the LED panel, a suitable logic level shifter is required.

The [SCT2024](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf) operates at 4 V and uses pull-up resistors on its inputs — which can feed unsafe voltages back into the ESP32. To protect the microcontroller and ensure consistent communication, *all signal lines should go through a level shifter*, not just those that are at risk.

> [!WARNING]
> Some users have reported success without level shifting, but this is outside the specifications. Skipping it can lead to unstable behavior, or even permanent damage to the ESP32.

## 🛠️ Hardware considerations

The *IKEA Frekvens* hardware isn’t perfect, but there’s room for improvements for those who desire.

### Capacitors

The original [89F112](https://lceda.cn/components/89F112_aeaaa99e4cd44677a24b9884cee22ff3) chip lacked dimming capabilities, but the ESP32 enables full PWM dimming. Because PWM involves rapid switching, it can stress the power supply and introduce noise. Decoupling capacitors are recommended to smooth out these fluctuations, improving stability and preventing timing issues.

The [SCT2024 datasheet](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf) suggests a total of ≈≥85 µF, placed as close as possible to the [SCT2024](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf) chips and LEDs. Instead of a single large capacitor, it is generally better to use two smaller ones.

**Easily accessible soldering points:**

- `VCC` / `GND` pads — near the top of the panel
- `DC+` / `DC-` pads — near the middle

> [!NOTE]
> Capacitors are optional — consider adding if you already have some available, or if you notice flicker or instability.

## 🔧 Configuration

| Label      | Type           | Constant   |
| ---------- | -------------- | -----------|
| `LAK`      | Digital output | `PIN_CS`   |
| `CLK`      | SPI SCLK       | `PIN_SCLK` |
| `DA`       | SPI MOSI       | `PIN_MOSI` |
| `EN`       | Digital output | `PIN_OE`   |
| `SW1`      | Digital input  | `PIN_SW1`  |
| `SW`       | Digital input  | `PIN_SW2`  |
| `U3` pin 7 | Analog input   | `PIN_MIC`  |

### Power and ground

Supplies power to both logic and LEDs.

- `DC+`/`DC-` is intended to supply high current to the LEDs.
- `VCC`/`GND` is intended as outputs for low current components handling the logic.
- Both are tied together internally on the PCB.

> [!CAUTION]
> To prevent backfeeding, never connect the ESP32 to USB while the 4 V power supply is connected — even if it is unplugged from the mains.

### SPI CS

Chip Select for the LED drivers.

Any digital output pin can be used.

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define PIN_CS 1 // LAK
```

> [!NOTE]
> [Logic level shifter](#%EF%B8%8F-logic-level-shifter) recommended.

> [!WARNING]
> Avoid strapping pins as this pin is pulled *LOW* with 25 kΩ resistance. On ESP32 classic it is recommended to use specialized pins, such as `CS` (sometimes labeled `SS` on older boards).

### SPI SCLK

Serial clock for SPI communication.

Any SPI `SCLK` pin can be used.

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define PIN_SCLK 2 // CLK
```

> [!NOTE]
> [Logic level shifter](#%EF%B8%8F-logic-level-shifter) recommended.

### SPI MOSI

Master-out data line for SPI.

Any SPI `MOSI` pin can be used.

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define PIN_MOSI 3 // DA
```

> [!NOTE]
> [Logic level shifter](#%EF%B8%8F-logic-level-shifter) recommended.

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

### Buttons

Button inputs for user interaction.

Optional to connect. Use RTC-capable digital input pins for best compatibility.

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define PIN_SW1 5 // SW1
#define PIN_SW2 6 // SW
```

> [!IMPORTANT]
> Avoid strapping pins as these is pulled *LOW* when pressed.

### Microphone amplifier

Analog input from the microphone amplifier.

Optional to connect. Use an ADC1-channel analog pin for best compatibility.

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define PIN_MIC 7 // U3 pin 7
```

> [!WARNING]
> Avoid strapping pins as this pin is biased.

## 📝 Template

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
# Device type
IKEA_FREKVENS='true'

# Names
NAME='Frekvens'
HOSTNAME='frekvens'
```

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#pragma once

// GPIO pins
#define PIN_CS 1   // LAK
#define PIN_SCLK 2 // CLK
#define PIN_MOSI 3 // DA
#define PIN_OE 4   // EN
#define PIN_SW1 5  // SW1
#define PIN_SW2 6  // SW
#define PIN_MIC 7  // U3 pin 7

// Wi-Fi credentials
#define WIFI_SSID "name"
#define WIFI_KEY "secret"

// Weather location (optional)
#define LATITUDE "0.000"
#define LONGITUDE "0.000"
```

## 🔗 Resources

A collection of external links for deeper exploration — including teardowns, mods, datasheets, and community projects — provided for reference only and with no formal connection to this project.

- [GitHub: attowatt/frekvensHack](https://github.com/attowatt/frekvensHack/blob/main/README.md)
- [GitHub: CrazyRobMiles/Frekvens-stuff-from-Rob](https://github.com/CrazyRobMiles/Frekvens-stuff-from-Rob/blob/master/README.md)
- [GitHub: frumperino/FrekvensPanel](https://github.com/frumperino/FrekvensPanel/blob/master/readme.md)
- [Hackaday: FREKVENS FJÄRRKONTROLL](https://hackaday.io/project/171034-frekvens-fjrrkontroll)
- [IKEA: FREKVENS Press kit](https://www.ikea.com/us/en/files/pdf/27/28/27281cda/frekvens_pr_kit.pdf)
- [IKEA: IKEA and Teenage Engineering team up for the FREKVENS collection](https://www.ikea.com/global/en/newsroom/collaborations/make-some-noise-and-get-the-party-started-with-upcoming-collection-frekvens--ikea-and-teenage-engineering-in-collaboration-170607/)
- [Imgur: FREKVENS HACKING](https://imgur.com/a/frekvens-hacking-Lk9bdTw)
- [Imgur: IKEA FREKVENS Matrix Teardown](https://imgur.com/gallery/ikea-frekvens-matrix-teardown-OBj0aC7)
- [Pixelkönig Blog: Y-Clock (IKEA FREKVENS HACK)](http://blog.digital-image.de/2023/08/17/y-clock/)
- [Reddit: FREKVENS LED Matrix Aux in replacement (or add on) for the microphone Tutorial (kinda)](https://www.reddit.com/r/FREKVENS/comments/16p6lbj/frekvens_led_matrix_aux_in_replacement_or_add_on/)
- [Sprites mods: Frekvens LED box hack](https://spritesmods.com/?art=frekvens)
- [StarChips: SCT2024 DataSheet](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf)
- [Teenage Engineering: Frekvens](https://teenage.engineering/designs/frekvens)
- [YouTube: REVIEW & TEARDOWN : IKEA FREKVENS sound to light box (a QUICK-VID)](https://www.youtube.com/watch?v=ZhBbMjyGwKM)
- [嘉立创EDA: 89F112](https://lceda.cn/components/89F112_aeaaa99e4cd44677a24b9884cee22ff3)
