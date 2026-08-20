# 🟢 Status LED

Provides low-level visual feedback of the device state. Primarily intended for basic troubleshooting when the device does not behave as expected.

The typical use case is the onboard LED commonly found on ESP32 boards, but any GPIO-connected LED can be used. A good starting point is a 5 mm LED-diode (e.g. [this](https://www.adafruit.com/product/299)) paired with a 100 Ω through-hole resistor (e.g. [this](https://www.adafruit.com/product/4293)). For a clean installation, the LED can be placed inside a standard LED holder (e.g. [this](https://www.adafruit.com/product/2175/)).

> [!NOTE]
> Some boards use an inverted onboard LED, meaning it lights up when turned off in software, and turns off when set to on.

## 📌 Schematics

### LED schema

```text
┌─────────┐
│   Anode ├─ Bridge
│ Cathode ├─ 0 V DC
└─────────┘
```

### Resistor schema

```text
 ┌── Bridge
┌┴┐
│ │
└┬┘
 └── LED
```

### ESP32 schema

```text
┌────────────────┐
│            3V3 ├─ +3.3 V DC
│            GND ├─ 0 V DC
│                │
│ Digital output ├─ LED
└────────────────┘
```

## 🔧 Configuration

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define PIN_LED 1
```

## 🧩 Extension

With the [Status LED](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-status-led) extension enabled, the LED reflects device state:

- Fast blink — error condition
- Slow blink — warning or connectivity issue
- Solid on — idle/normal operation
