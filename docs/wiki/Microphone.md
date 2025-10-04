# 🎙️ Microphone

Some [modes](https://github.com/VIPnytt/Frekvens/wiki/Modes) can react to sound — for example, syncing animations with music.

Most analog microphone modules with a built-in amplifier will work. A good starting point is a electret microphone (e.g. [BOB-12758](https://www.sparkfun.com/sparkfun-electret-microphone-breakout.html)) or an MEMS microphone (e.g. [SPW2430](https://www.adafruit.com/product/2716)).

## 📌 Schematics

### Amplifier schema

```text
┌─────┐
│ VCC ├─ +3.3 V DC
│ GND ├─ 0 V DC
│     │
│ OUT ├─ Amplifier
└─────┘
```

### ESP32 schema

```text
┌──────────────┐
│          3V3 ├─ +3.3 V DC
│          GND ├─ 0 V DC
│              │
│ Analog input ├─ Amplifier
└──────────────┘
```

## 🔧 Configuration

### Microphone amplifier

Analog input from the microphone amplifier.

Any analog input pin can be used, but those on the ADC1 channel are preferred.

> Avoid strapping pins as this pin is biased.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_MIC 1 // Amplifier
```

## 🧩 Extension

The [Microphone](https://github.com/VIPnytt/Frekvens/wiki/Extensions#️-microphone) extension provides the capability for various [modes](https://github.com/VIPnytt/Frekvens/wiki/Modes) to synchronize with music.

## ✅ IKEA Frekvens

[IKEA Frekvens](https://github.com/VIPnytt/Frekvens/wiki/IKEA-Frekvens#wiring-the-microphone) has an microphone built-in.

On the green PCB there’s an [LM358](https://www.onsemi.com/download/data-sheet/pdf/lm358-d.pdf) amplifier, simply connect `U3` pin 7 to an analog input on the ESP32 board.
Since the [89F112](https://lceda.cn/components/89F112_aeaaa99e4cd44677a24b9884cee22ff3) chip should be desoldered, it might be easier to connect from `U2` pad 11 instead.

## 🔗 Resources

External links for deeper exploration — provided for reference only and with no formal connection to this project.

- [GitHub: attowatt/frekvensHack](https://github.com/attowatt/frekvensHack/blob/main/README.md)
- [Reddit: FREKVENS LED Matrix Aux in replacement (or add on) for the microphone Tutorial (kinda)](https://www.reddit.com/r/FREKVENS/comments/16p6lbj/frekvens_led_matrix_aux_in_replacement_or_add_on/)
