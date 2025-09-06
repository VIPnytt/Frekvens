# 💡 IKEA Frekvens LED multi-use light

> Article number 504.203.53

## 📈 Schematics

### LED panel schema

```text
             ┌────────────────────── SPI CS
             │   ┌────────────────── SPI SCLK
             │   │   ┌────────────── SPI MOSI
             │   │   │   ┌────────── Enable
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
└──────────────┼───────────┼───┼──┘
               │           │   └──── +4 V DC
               │           └──────── 0 V DC
               └──────────────────── SPI MISO
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
              ┌──────┐
100-240 V AC ─┤ L  + ├─ +4 V DC
     Neutral ─┤ N  - ├─ 0 V DC
              └──────┘
```

### ESP32 schema

```text
┌────────────────┐
│            VIN ├─ +4 V DC
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
 SPI SCLK ─┤             ├─ SPI SCLK
 SPI MISO ─┤             ├─ SPI MISO
 SPI MOSI ─┤             ├─ SPI MOSI
   SPI CS ─┤             ├─ SPI CS
   Enable ─┤             ├─ Enable
           └─────────────┘
```

## 🚀 Getting started

### Opening up

After loosening the four screws and opening the back panel, there's eight nuts held in place by four plastic clips. Once those and the four screws in the bottom are removed, everything slides out easily.

### Disconnect the buttons

The next step is to disconnect the buttons from the green PCB, `SW1`, `SW2` and `COM`.

### Removing the `U2` chip

This step can be a bit challanging: removing the [89F112](https://lceda.cn/components/89F112_aeaaa99e4cd44677a24b9884cee22ff3) chip labeled `U2` from the green PCB. This chip is responsible for handling the integrated microphone, button inputs and display output.

> If the integrated microphone is not needed, the entire green PCB can be removed from the LED panel. This makes the modification easier, while still allowing the board to be re-soldered later if microphone support is desired.

### Wiring the microphone

For the microphone to be functional, connect a wire from the [LM358](https://www.onsemi.com/download/data-sheet/pdf/lm358-d.pdf) amplifier labeled `U3` pin 7 to an *analog input* on the ESP32.

> Since the [89F112](https://lceda.cn/components/89F112_aeaaa99e4cd44677a24b9884cee22ff3) chip should be desoldered, it might be easier to connect from `U2` pad 11 instead. It's internally connected to `U3` pin 7 via traces on the PCB.

### The Missing IR sensor

The device was originally designed with an IR sensor in mind, but the feature was never implemented in production — leaving the `IR` pad unused.

The hole next to `C10` was reserved for this purpose and can still be used to mount an [IR sensor](https://github.com/VIPnytt/Frekvens/wiki/Infrared), enabling a capability that was planned in the original design but never realized.

### Wiring the LED panel

Next up is attaching the ESP32, via the [logic level shifter](#%EF%B8%8F-logic-level-shifter). There's two noteworthy locations, the first one is on top of the green PCB, and the second one is the six unlabeled pads at bottom left.

> The two left-most pads will be blocked by the chassis.

### Connecting the button

The last step is to connect the buttons: `SW` and `SW1` each connect to separate pins on the ESP32, while `COM` connects to `GND`.

> On the stock wiring harness, the red *power* button `SW1` uses the black wire, the yellow *mode* button `SW` uses the white wire, and both share a red wire for `COM`.

## ↔️ Logic level shifter

For safe and reliable communication between the ESP32 and the LED panel, a suitable logic level shifter is required.

The [SCT2024](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf) outputs 4 V signals and also uses pull-ups on its inputs — both of which can feed unsafe voltages back into the ESP32. To protect the microcontroller and ensure consistent communication, *all signal lines should go through a level shifter*, not just those that are at risk.

> Some users have reported success without level shifting, but this is outside the specifications. Skipping it can lead to unstable behavior, or even permanent damage to the ESP32.

## 🛠️ Hardware considerations

The *IKEA Frekvens* hardware isn't perfect, but there's room for improvements for those who desire.

### Capacitors

The original [89F112](https://lceda.cn/components/89F112_aeaaa99e4cd44677a24b9884cee22ff3) chip lacked dimming capabilities, but the ESP32 enables full PWM dimming. Because PWM involves rapid switching, it can stress the power supply and introduce noise. Decoupling capacitors are recommended to smooth out these fluctuations, improving stability and preventing timing issues.

The [SCT2024 datasheet](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf) suggests a total of ≈≥85 µF, placed as close as possible to the [SCT2024](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf) chips and LEDs. Instead of a single large capacitor, it is generally better to use two smaller ones.

**Easily accessible soldering points:**

- `VCC` / `GND` pads — near the top of the panel
- `DC+` / `DC-` pads — near the middle

> Capacitors are optional — consider adding them if you already have some available, or if you notice flicker or instability.

## 🔧 Configuration

| Frekvens    | Via                 | ESP32          | Function  | Constant |
| ----------- | ------------------- | -------------- | --------- | ---------|
| `CLK`       | Logic level shifter | SPI SCLK       | SPI SCLK  | `PIN_SCLK` |
| `DA` output | Logic level shifter | SPI MISO       | SPI MISO  | `PIN_MISO` |
| `DA` input  | Logic level shifter | SPI MOSI       | SPI MOSI  | `PIN_MOSI` |
| `LAK`       | Logic level shifter | Digital output | SPI CS    | `PIN_CS`   |
| `EN`        | Logic level shifter | PWM output     | Enable    | `PIN_EN`   |
| `SW1`       |                     | Digital input  | Button 1  | `PIN_SW1`  |
| `SW`        |                     | Digital input  | Button 2  | `PIN_SW2`  |
| `U3` pin 7  |                     | Analog input   | Amplifier | `PIN_MIC`  |

### Power and ground

- `DC+`/`DC-` is intended to supply high current to the LEDs.
- `VCC`/`GND` is intended as outputs for low current components handling the logic.
- Both are tied together internally on the PCB.

> To prevent backfeeding, never connect the ESP32 to USB while the 4 V power supply is connected — even if it is unplugged from the mains.

### SPI SCLK

[Logic level shifter](#%EF%B8%8F-logic-level-shifter) recommended.

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

[Logic level shifter](#%EF%B8%8F-logic-level-shifter) recommended.

Any *SPI `MOSI`* pin can be used.

> The use of either the `HSPI` or `VSPI` bus is required for consistency on boards with two SPI interfaces.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_MOSI 3 // GPIO #
```

### SPI CS

[Logic level shifter](#%EF%B8%8F-logic-level-shifter) recommended.

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

### Buttons

Optional to connect.

Any *digital input* pins can be used, but those that are also RTC-capable are preferred.

> Avoid strapping pins as these is pulled *LOW* when pressed.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_SW1 6 // GPIO #
#define PIN_SW2 7 // GPIO #
```

### Amplifier

Optional to connect.

Any *analog input* pin can be used, but those on the ADC1 channel are preferred.

> Avoid strapping pins as this pin is biased. On ESP32 (LX6-based, original series) boards, the ADC2 channel pins are not supported.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_MIC 8 // GPIO #
```

## 📝 Template

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
# Device type
ENV_FREKVENS=''

# Custom device name (optional)
NAME='Frekvens'
```

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#pragma once

// GPIO pins
#define PIN_SCLK 1
#define PIN_MOSI 2
#define PIN_CS 3
#define PIN_EN 4
#define PIN_SW1 5
#define PIN_SW2 6
#define PIN_MIC 7

// Wi-Fi credentials (optional)
#define WIFI_SSID "name"
#define WIFI_KEY "secret"

// Weather location (optional)
#define LATITUDE "0.000"
#define LONGITUDE "0.000"
```
