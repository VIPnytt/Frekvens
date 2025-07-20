# 🎙️ Microphone

Many of the [modes](Modes.md) has the ability to react to sounds, eg. syncing animations up with the music.
It's worth noting that it's not possible to record audio, as only voltage levels can be read by the ESP32's *analog input* pins, not frequencies.

Most analog microphones paired with an amplifier can be used, eg. [SPW2430](https://www.adafruit.com/product/2716) or [MAX4466](https://www.adafruit.com/product/1063).

## 📈 Schematics

### Microphone schematics

```text
┌─────┐
│ VCC ├─ +3.3 V DC
│ GND ├─ 0 V DC
│     │
│ OUT ├─ Signal
└─────┘
```

### ESP32 schematics

```text
┌──────────────┐
│          3V3 ├─ +3.3 V DC
│          GND ├─ 0 V DC
│              │
│ Analog input ├─ Signal
└──────────────┘
```

## 🔧 Configuration

### Signal

Use any *analog input* pin.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_MIC 1 // GPIO #
```

## 🧩 Extension

Using the [Microphone](Extensions#️-microphone) extension, various [modes](Modes) can sync up with the music.

Check out the [Microphone](Extensions#️-microphone) extension for more info.

## ✅ IKEA Frekvens

The device has an MEMS microphone built-in.
On the green PCB there's an [LM358](https://www.onsemi.com/download/data-sheet/pdf/lm358-d.pdf) amplifier, simply connect `U3` pin 7 to an analog input on the ESP32.
Since the [89F112](https://lceda.cn/components/89F112_aeaaa99e4cd44677a24b9884cee22ff3) should be removed anyway, it might be easier to conenct from `U2` pad 11 instead.

Check out the [IKEA Frekvens](Frekvens.md#️-wiring-the-microphone) wiki for more info.
