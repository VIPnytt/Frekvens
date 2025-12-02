# 🔆 Photocell

Automatic ambient brightness adjustment can be achieved by installing a Light Dependent Resistor (LDR). This simple sensor changes resistance with light levels, allowing the ESP32 to measure surrounding brightness and adapt the display accordingly.

Most common LDRs work well when paired with a reasonably sized fixed resistor. They are inexpensive and widely available from hobby and electronics suppliers (e.g. [CdS photoresistor](https://www.adafruit.com/product/161) and [Through-Hole Resistors](https://www.adafruit.com/product/2892)).

A **10 kΩ resistor** is a good starting point, but for best results check the LDR’s datasheet and select a value that matches the typical daylight conditions where the device will operate.

For a clean installation, the LDR can be placed inside a standard LED cap (e.g. [LC5-8](https://www.toby.co.uk/led-and-light-pipes/led-mounts-and-spacers/lc3-and-lc5-valcon-3mm-and-5mm-led-lens-caps/)) so it mounts neatly in the device chassis and remains visually discreet.

## 📌 Schematics

### LDR schema

```text
┌─┐
│ ├─ +3.3 V DC
│ ├─ Bridge
└─┘
```

### Resistor schema

```text
 ┌── Bridge
┌┴┐
│ │
└┬┘
 └── 0 V DC
```

### ESP32 schema

```text
┌──────────────┐
│          3V3 ├─ +3.3 V DC
│          GND ├─ 0 V DC
│              │
│ Analog input ├─ Bridge
└──────────────┘
```

## 🔧 Configuration

### Sensor reading

Analog input from the resistor bridge.

Any analog input pin can be used, but those on the ADC1 channel are preferred.

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define PIN_LDR 1 // Bridge
```

> [!WARNING]
> Avoid strapping pins as this pin is biased.

## 🧩 Extension

With the [Photocell](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-photocell) extension enabled, the device automatically adjusts display brightness to match ambient light throughout the day.

## 🔗 Resources

External links for deeper exploration — provided for reference only and with no formal connection to this project.

- [Adafruit: Photocells](https://learn.adafruit.com/photocells?view=all)
- [Pixelkönig Blog: X-Clock (IKEA OBEGRÄNSAD Hack)](http://blog.digital-image.de/2023/05/31/x-clock/)
- [Pixelkönig Blog: Y-Clock (IKEA FREKVENS HACK)](http://blog.digital-image.de/2023/08/17/y-clock/)
