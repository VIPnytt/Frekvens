# 💡 IKEA Frekvens LED spotlight

> Article number 504.203.67

## 📈 Schematics

### PCB schema

```text
         ┌──────────────────────────────────┐
         │     ┌───────────────────┬────────┼─ Amplifier
 0 V DC ─┼ DC- │    ┌───────┐      │ ┌─┐    │
+4 V DC ─┼ DC+ │    │┌────┐ │      │ ┤ ├    │
         │     │ U2 ││    │ │      │ ┤ ├    │
         │     │   ┌┴┴┴┴┐ │ │  U3  └─┤ ├    │
         │     │   └┬┬┬┬┘ │ │  LM358 ┤ ├    │
         │     └────┘     │ │        └─┘    │
     OE ─┼────────┬───────┤ └─────┬───────┬─┼─ Button
         │        │  ┌─┐  │  ┌─┐  │  ┌─┐  │ │
         │        │  ┤ ├  │  ┤ ├  │  ┤ ├  │ │
         │        │  ┤ ├  │  ┤ ├  └──┤ ├──┘ │
         │        └──┤ ├  └──┤ ├  K3 └─┘    │
         │        U6 ┤ ├  U7 ┤ ├            │
         │           └─┘     └─┘       MIC- │
         │ LED+ LED-                   MIC+ ┼─ Microphone
         └───────┼──────────────────────────┘
    LED ─────────┘
```

### Microphone schema

```text
┌─────┐
│ VCC ├─ Microphone
│ GND ├─ 0 V DC
└─────┘
```

### LED diode schema

```text
┌────────┐
│  Anode ├─ +4 V DC
│ Catode ├─ LED
└────────┘
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
┌───────────────┐
│           VIN ├─ +4 V DC
│           GND ├─ 0 V DC
│               │
│    PWM output ├─ OE
│               │
│ Digital input ├─ Button
│               │
│  Analog input ├─ Amplifier
└───────────────┘
```

## 🚀 Getting started

First, desolder the `U2` chip and wire up the ESP32 according to the provided schematics.

Refer to the [PCB schema](#pcb-schema) above for the internal connections of each component on the board — this will allow you to choose a convenient soldering point.

## 🔧 Configuration

### Power and ground

The `DC+`/`LED+` and `DC-`/`MIC-` pins are internally connected via traces on the PCB.

> To prevent backfeeding, never connect the ESP32 to USB while the 4 V power supply is connected — even if it is unplugged from the mains.

### Output Enable

Any PWM output pin can be used.

> Avoid strapping pins as this pin may be floating.

```yaml
PIN_LED: 1 # U6/U7
```

### Button

Any digital input pin can be used, but those that are also RTC-capable are preferred.

> Avoid strapping pins as this pin is pulled *HIGH* using a resistor and *LOW* when pressed.

```yaml
PIN_SW1: 2 # K3
```

### Amplifier

Any analog input pin can be used, but those on the ADC1 channel are preferred.

> Avoid strapping pins as this pin is biased. On ESP32 (LX6-based, original series) boards, the ADC2 channel pins are not supported.

```yaml
PIN_MIC: 3 # U3
```
