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
│ │         LM358 ┤ ├──┴────────┼─┼─ Microphone
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
│   Analog input ├─ Microphone
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

After loosening the four screws and opening the back panel, there's eight nuts held in place by four plastic clips. Once those and the four screws in the bottom are removed, everything slides out nicely.

### Disconnect the buttons

The next step is to disconnect the buttons from the green PCB, `SW1`, `SW2` and `COM`.

### Removing the `U2` chip

This step can be a bit challanging, removing the [89F112](https://lceda.cn/components/89F112_aeaaa99e4cd44677a24b9884cee22ff3) chip labeled `U2` from the green PCB. This chip is the core handling the mic and button inputs as well as the display output.

Alternatively, if the microphone functionality isn't necessary, just remove the green PCB from whe white PCB altogether. The green PCB is only needed for the microphone, and can be re-soldered back on later, if desired.

### Wiring the microphone

For the microphone to be functional, connect a wire from the [LM358](https://www.onsemi.com/download/data-sheet/pdf/lm358-d.pdf) amplifier labeled `U3` pin 7 to an analog input on the ESP32.
Since the [89F112](https://lceda.cn/components/89F112_aeaaa99e4cd44677a24b9884cee22ff3) chip labeled `U2` now is removed, it might be easier to connect to `U2` pad 11 instead. It's internally connected to `U3` pin 7 via traces on the PCB.

### The missing IR-receiver

When it comes to the IR-pad, it's worth noting that *IKEA Frekvens* never shipped with an IR-receiver sensor, so this pad has no function.

### Wiring the LED panel

Next up is attaching the ESP32, via the [logic level shifter](#%EF%B8%8F-logic-level-shifter). There's two noteworthy locations, the first one is on top of the green PCB, and the second one is the six unlabeled pads at bottom left where two of the left-most pads will be blocked by the chassis.

### Connecting the button

The last step is to connect the buttons, `SW` and `SW1` both connects to separate pins on ESP32, while the third `COM` connects to ground.

## ↔️ Logic level shifter

The [ESP32](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf) operates at 3.3 V logic, while the [SCT2024](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf) operates at 4 V logic. Therefore, a bidirectional logic level shifter compatible with SPI, such as the [TXB0104](https://www.adafruit.com/product/1875) or [TXB0108](https://www.adafruit.com/product/395), is required to ensure reliable and safe communication between the devices.

- The [SCT2024](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf)'s logic *HIGH* level is above [ESP32](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)'s absolute maximum voltage rating on its GPIO pins:
  - SPI MISO *`DA`*
  - Enable *`EN`*

It is considered best practice to keep all logic signals at a consistent voltage level within each interface. Therefore, it is recommended to use a logic level shifter on all relevant signal lines, even if not strictly required.

> While there are reports suggesting that these devices can work together without level shifting, this is strongly discouraged. Exceeding the [ESP32](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)’s voltage ratings may result in unpredictable behavior or permanent damage to the microcontroller.

## 🛠️ Hardware considerations

The *IKEA Frekvens* hardware isn't perfect, but there's room for improvements for those who desire.

### Capacitors

For dimming of the display, the [SCT2024](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf) datasheet suggests adding an additional *"4.7 µF or more"* depending on the LED load current (~2.78 mA/output), for every chip. With 16 chips in total, that's a minimum of 75.2 µF. The same datasheet also recommends adding *"greater than 10 µF"* besides the LEDs, bringing the total up to 85.2 µF.

The capacitors mentioned aren't present on the PCB, but will help prevent timing issues, as well as minimize noise from the power supply. Ideally they should be placed as close to the load as possible.

**Suggestion:**

- Add at least 85.2 µF, eg. [100 µF](https://www.adafruit.com/product/2193)

**Tip:** The top `VCC` and `GND` pads are good positions for capacitors. Another option is the `DC+` and `DC-` pads.

## 🔧 Configuration

| Frekvens    | Via                 | ESP32          | Function   | Constant |
| ----------- | ------------------- | -------------- | ---------- | ---------|
| `CLK`       | Logic level shifter | SPI SCLK       | SPI SCLK   | `PIN_SCLK` |
| `DA` output | Logic level shifter | SPI MISO       | SPI MISO   | `PIN_MISO` |
| `DA` input  | Logic level shifter | SPI MOSI       | SPI MOSI   | `PIN_MOSI` |
| `LAK`       | Logic level shifter | Digital output | SPI CS     | `PIN_CS`   |
| `EN`        | Logic level shifter | PWM output     | Enable     | `PIN_EN`   |
| `SW1`       |                     | Digital input  | Button 1   | `PIN_SW1`  |
| `SW`        |                     | Digital input  | Button 2   | `PIN_SW2`  |
| `U3` pin 7  |                     | Analog input   | Microphone | `PIN_MIC`  |

### Power and ground

- `DC+` and `DC-` is intended to supply high current to the LEDs.
- `VCC` and `GND` is intended as outputs for low current components handling the logic.
- Both are tied together internally on the PCB.

> To prevent backfeeding, never connect the ESP32 to USB while the 4 V power supply is connected — even if it is unplugged from the mains.

### SPI SCLK

[Logic level shifter](#%EF%B8%8F-logic-level-shifter) recommended.

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

[Logic level shifter](#%EF%B8%8F-logic-level-shifter) recommended.

Use any *SPI `MOSI`* pin.

> If the board has two sets of SPI pins, choose any of them, but be consistent and always use either `HSPI` or `VSPI`.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_MOSI 3 // GPIO #
```

### SPI CS

[Logic level shifter](#%EF%B8%8F-logic-level-shifter) recommended.

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

### Buttons

Optional to connect.

Use any *digital input* pins, preferably those that are also RTC-capable, to allow deep sleep wake-up functionality.

> Avoid strapping pins as these is be pulled *LOW* when pressed.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_SW1 6 // GPIO #
#define PIN_SW2 7 // GPIO #
```

> `COM` must be connected to ground.

### Microphone

Optional to connect.

Use any *analog input* pin.

> Avoid strapping pins as this pin is biased.

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
