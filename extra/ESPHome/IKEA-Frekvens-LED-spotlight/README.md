# 💡 IKEA Frekvens LED spotlight

> Article number 504.203.67

## 📌 Schematics

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

### LED diode schema

```text
┌────────┐
│  Anode ├─ +4 V DC
│ Catode ├─ LED
└────────┘
```

### Microphone schema

```text
┌─────┐
│ VCC ├─ Microphone
│ GND ├─ 0 V DC
└─────┘
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
│            GND ├─ 0 V DC
│                │
│ Digital output ├─ OE
│                │
│  Digital input ├─ Button
│                │
│   Analog input ├─ Amplifier
└────────────────┘
```

## 🚀 Getting started

First, desolder the `U2` chip and wire up the ESP32.

Refer to the [PCB schema](#pcb-schema) above for the internal connections of each component on the board — this will allow you to choose a convenient soldering point.

## 🔧 Configuration

### Power and ground

Supplies power to both logic and the LED.

The `DC+`/`LED+` and `DC-`/`MIC-` pins are internally connected via traces on the PCB.

> To prevent backfeeding, never connect the ESP32 to USB while the 4 V power supply is connected — even if it is unplugged from the mains.

### Output Enable

Enables or disables LED output.

Any digital output pin can be used.

> Avoid strapping pins as this pin may be floating.

```yaml
PIN_OE: 1 # U6/U7
```

### Button

Button inputs for user interaction.

Use digital input pins, preferably RTC-capable.

> Avoid strapping pins as this pin is pulled *HIGH* using a resistor and *LOW* when pressed.

```yaml
PIN_SW: 2 # K3
```

### Microphone amplifier

Analog input from the microphone amplifier.

Use an ADC1-channel analog pin for best compatibility.

> Avoid strapping pins as this pin is biased.

```yaml
PIN_MIC: 3 # U3
```

## 🔗 Resources

External links for deeper exploration — provided for reference only and with no formal connection to this project.

- [IKEA: FREKVENS PR kit](https://www.ikea.com/us/en/files/pdf/27/28/27281cda/frekvens_pr_kit.pdf)
- [Instagram: Simple IKEA FREKVENS Hack](https://www.instagram.com/p/B8cVZQCo1BH/)
