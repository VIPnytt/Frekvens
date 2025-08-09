# 💡 IKEA Frekvens LED spotlight

> Article number 504.203.67

## 📈 Schematics

### PCB schema

```text
         ┌──────────────────────────────────┐
         │     ┌───────────────────┬────────┼─ Microphone
 0 V DC ─┼ DC- │    ┌───────┐      │ ┌─┐    │
+4 V DC ─┼ DC+ │    │┌────┐ │      │ ┤ ├    │
         │     │ U2 ││    │ │      │ ┤ ├    │
         │     │   ┌┴┴┴┴┐ │ │  U3  └─┤ ├    │
         │     │   └┬┬┬┬┘ │ │  LM358 ┤ ├    │
         │     └────┘     │ │        └─┘    │
 Enable ─┼────────┬───────┤ └─────┬───────┬─┼─ Button
         │        │  ┌─┐  │  ┌─┐  │  ┌─┐  │ │
         │        │  ┤ ├  │  ┤ ├  │  ┤ ├  │ │
         │        │  ┤ ├  │  ┤ ├  └──┤ ├──┘ │
         │        └──┤ ├  └──┤ ├  K3 └─┘    │
         │        U6 ┤ ├  U7 ┤ ├            │
         │           └─┘     └─┘       MIC- │
         │ LED+ LED-                   MIC+ ┼─ Signal
         └───────┼──────────────────────────┘
    LED ─────────┘
```

### Microphone schema

```text
┌─────┐
│ VCC ├─ Signal
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
┌────────────────┐
│            VIN ├─ +4 V DC
│            GND ├─ 0 V DC
│                │
│     PWM output ├─ Enable
│                │
│  Digital input ├─ Button
│                │
│   Analog input ├─ Microphone
└────────────────┘
```

## 🚀 Getting started

Remove the `U2` chip, then connect each component to the ESP32 using wires.

Refer to the [PCB schema](#pcb-schema) above for the internal connections of each component on the board — this allows you to solder the wire to any convenient point on the PCB.

## 🔧 Configuration

### Power and ground

> To prevent backfeeding, never connect the ESP32 to USB while the 4 V power supply is connected — even if it is unplugged from the mains.

### Enable

Use any *PWM output* pin.

> Avoid strapping pins as this pin may be floating.

### Button

Use any *digital input* pin, preferably those that are also RTC-capable, to allow deep sleep wake-up functionality.

> Avoid strapping pins as this pin is pulled *HIGH* using a resistor and *LOW* when pressed.

### Microphone

Use any *analog input* pin.

> Avoid strapping pins as this pin is biased.
