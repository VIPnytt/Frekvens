# 🔴 Infrared

By adding an IR sensor, you can use a remote control, like a TV remote, to operate the device. This gives you the flexibility to use an existing remote you may already own.

Most remotes are using the 38 kHz frequency band.
While virtually *any* IR sensor can be used, the simplest way to getting started might be to use an [TSOP382](https://www.adafruit.com/product/157).

## 📈 Schematics

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
┌────────────────┐
│            VIN ├─ +5 V DC
│            3V3 ├─ +3.3 V DC
│            GND ├─ 0 V DC
│                │
│  Digital input ├─ Data
└────────────────┘
```

### Logic level shifter schema

```text
   0 V DC ────────┬──────── 0 V DC
+3.3 V DC ────┐   │   ┌──── +5 V DC
           ┌──┴───┴───┴──┐
           │ VCC GND VCC │
    Data  ─┤             ├─ Data
           └─────────────┘
```

## ↔️ Logic level shifter

Components connected to voltage levels above 3.3 V often requires an voltage-level translator to remain compatible with ESP32's logic pins.
Most IR-remote sensors are 3.3 V compatible, but if 5 V logic is desired, the easiest solution might be to use an [TXB0104](https://www.adafruit.com/product/1875) or [TXB0108](https://www.adafruit.com/product/395).

## 🔧 Configuration

### Data

Any *digital input* pin can be used.

> Avoid **strapping** pins as this pin is pulled *HIGH* when idle.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_IR 1 // GPIO #
```

## 🧩 Extension

Using the [Infrared](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-infrared) extension, the device can be controlled using a remote control.

Check out the [Infrared](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-infrared) extension for more info.

## 🚫 IKEA Frekvens

The device was designed with IR capabilities in mind, but shipped without.
