# 💡 IKEA Frekvens LED spotlight

Article number: `504.203.67`

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
┌─────────┐
│   Anode ├─ +4 V DC
│ Cathode ├─ LED
└─────────┘
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

## 🏠 ESPHome

This guide is intended for use with [ESPHome](https://esphome.io), using the [`esphome.yaml`](https://github.com/VIPnytt/Frekvens/blob/main/extra/IKEA-Frekvens-LED-spotlight/esphome.yaml) template. Feel free to change the configuration as desired, including adding or removing functionality.

## 🚀 Getting started

First, desolder the `U2` chip and wire up the ESP32.

Refer to the [PCB schema](#pcb-schema) above for the internal connections of each component on the board — this will allow you to choose a convenient soldering point.

## 🔧 Configuration

### Power and ground

Supplies power to both logic and the LED.

The `DC+`/`LED+` and `DC-`/`MIC-` pins are internally connected via traces on the PCB.

> [!CAUTION]
> To prevent backfeeding, never connect the ESP32 to USB while the 4 V power supply is connected — even if it is unplugged from the mains.

### Output Enable

Enables or disables LED output.

Any digital output pin can be used.

Configure under `substitutions` in the ESPHome YAML:

```yaml
PIN_OE: 1 # U6/U7
```

> [!IMPORTANT]
> Avoid strapping pins as this pin may be floating.

### Button

Button input for user interaction.

Use digital input pins, preferably RTC-capable.

Configure under `substitutions` in the ESPHome YAML:

```yaml
PIN_SW: 2 # K3
```

> [!WARNING]
> Avoid strapping pins as this pin is pulled *HIGH* by default and *LOW* when pressed.

### Microphone amplifier

Analog input from the microphone amplifier.

Use an ADC1-channel analog pin for best compatibility.

Configure under `substitutions` in the ESPHome YAML:

```yaml
PIN_MIC: 3 # U3
```

> [!WARNING]
> Avoid strapping pins as this pin is biased.

## 🔗 Resources

External links for deeper exploration — provided for reference only and with no formal connection to this project.

- [IKEA: FREKVENS Press kit](https://www.ikea.com/us/en/files/pdf/27/28/27281cda/frekvens_pr_kit.pdf)
- [IKEA: Get the party started with FREKVENS](https://www.ikea.com/us/en/newsroom/range-news/get-the-party-started-with-frekvens-pubaaf1fd67/)
- [IKEA: IKEA and Teenage Engineering team up for the FREKVENS collection](https://www.ikea.com/global/en/newsroom/collaborations/make-some-noise-and-get-the-party-started-with-upcoming-collection-frekvens--ikea-and-teenage-engineering-in-collaboration-170607/)
- [Instagram: Simple IKEA FREKVENS Hack](https://www.instagram.com/p/B8cVZQCo1BH/)
- [Teenage Engineering: Frekvens](https://teenage.engineering/designs/frekvens)
