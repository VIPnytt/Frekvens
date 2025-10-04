# 🔴 IR receiver

By adding an IR receiver, you can control the device with almost any common remote — such as a TV remote you already own.

Most remotes operate around the 38 kHz frequency band. While many IR sensors and modules will work, a good starting point is the TSOP382 series (e.g. [TSOP38238](https://www.adafruit.com/product/157)).

## 📌 Schematics

### Infrared schema

```text
┌─────┐
│ VCC ├─ +3.3 V DC
│ GND ├─ 0 V DC
│     │
│ OUT ├─ Data
└─────┘
```

### ESP32 schema

```text
┌───────────────┐
│           VIN ├─ +5 V DC
│           3V3 ├─ +3.3 V DC
│           GND ├─ 0 V DC
│               │
│ Digital input ├─ Data
└───────────────┘
```

### Logic level shifter schema

```text
   0 V DC ────────┬──────── 0 V DC
+3.3 V DC ────┐   │   ┌──── +5 V DC
           ┌──┴───┴───┴──┐
           │ VCC GND VCC │
    Data  ─┤     ◄──     ├─ Data
           └─────────────┘
```

## ↔️ Logic level shifter

Most IR sensors are 3.3 V compatible, but some variants use higher logic levels (e.g. 5 V). If so, a logic level shifter is required to protect the ESP32. Unlike the high-speed SPI lines for the display, this signal is low-speed and single-channel — meaning almost any level-shifting method will work.

## 🔧 Configuration

### Data

Data-line for received signals.

Any digital input pin can be used.

> Avoid strapping pins as this pin is pulled *HIGH* when idle.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_IR 1 // Receiver
```

## 🧩 Extension

Using the [Infrared](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-infrared) extension, the device can be controlled using a remote control.

Check out the [Infrared](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-infrared) extension for more info.

## 🚫 IKEA Frekvens

The device was originally designed with an IR sensor in mind, but the feature was never implemented in production. The unused hole next to `C10` can, however, be repurposed for adding one.

## 🔗 Resources

External links for deeper exploration — provided for reference only and with no formal connection to this project.

- [Adafruit: IR Sensor](https://learn.adafruit.com/ir-sensor?view=all)
