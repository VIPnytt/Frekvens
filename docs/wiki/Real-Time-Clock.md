# ⏰ Real-Time Clock

A Real-Time Clock (RTC) module keeps accurate time, even without internet access.

Most common RTC modules will work. Good starting points include the [DS3231](https://www.adafruit.com/product/3013) and the [DS1307](https://www.adafruit.com/product/3296).

## ✅ Supported types

- [DS1307](https://www.adafruit.com/product/3296)
- [DS3231](https://www.adafruit.com/product/3013)
- [DS3232](https://www.dfrobot.com/product-2304.html)
- [PCF8563](https://www.waveshare.com/PCF8563-RTC-Board.htm)

## 📌 Schematics

### DS1307 schema

```text
┌─────┐
│ VCC ├─ +5 V DC
│ GND ├─ 0 V DC
|     |
│ SCL ├─ I²C SCL
│ SDA ├─ I²C SDA
└─────┘
```

### DS3231 schema

```text
┌─────┐
│ VCC ├─ +3.3 V DC
│ GND ├─ 0 V DC
|     |
│ SCL ├─ I²C SCL
│ SDA ├─ I²C SDA
|     |
│ INT ├─ RTC INT
└─────┘
```

### DS3232 schema

```text
┌─────┐
│ VCC ├─ +3.3 V DC
│ GND ├─ 0 V DC
|     |
│ SCL ├─ I²C SCL
│ SDA ├─ I²C SDA
|     |
│ INT ├─ RTC INT
└─────┘
```

### PCF8563 schema

```text
┌─────┐
│ VCC ├─ +3.3 V DC
│ GND ├─ 0 V DC
|     |
│ SCL ├─ I²C SCL
│ SDA ├─ I²C SDA
|     |
│ INT ├─ RTC INT
└─────┘
```

### ESP32 schema

```text
┌────────────────┐
│            VIN ├─ +5 V DC
│            3V3 ├─ +3.3 V DC
│            GND ├─ 0 V DC
│                │
│            SCL ├─ I²C SCL
│            SDA ├─ I²C SDA
│                │
│  Digital input ├─ RTC INT
└────────────────┘
```

### Logic level shifter schema

```text
   0 V DC ────────┬──────── 0 V DC
+3.3 V DC ────┐   │   ┌──── +5 V DC
           ┌──┴───┴───┴──┐
           │ VCC GND VCC │
 I²C SCL  ─┤     ──►     ├─ I²C SCL
 I²C SDA  ─┤     ◄─►     ├─ I²C SDA
 RTC INT  ─┤     ◄──     ├─ RTC INT
           └─────────────┘
```

## ↔️ Logic level shifter

Most RTC modules are 3.3 V compatible, but some variants use higher logic levels (e.g. 5 V). In those cases, a logic level shifter is required to ensure safe communication with the ESP32. Make sure the level shifter is suited for I²C signals.

## 🔧 Configuration

### I²C SCL

Serial clock for I²C communication.

Any I²C `SCL` pin can be used.

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define PIN_SCL 1 // I²C SCL
```

### I²C SDA

Bidirectional data line for I²C.

Any I²C `SDA` pin can be used.

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define PIN_SDA 2 // I²C SDA
```

### RTC INT

Interrupt signal from the RTC module.

Optional to connect.

Any digital input pin that are also RTC-capable can be used.

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define PIN_INT 3 // RTC INT
```

## 🧩 Extension

Using the [RTC](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-rtc) extension, the clock will automatically sync during during startup.

Check out the [RTC](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-rtc) extension for more info.

## 📝 Templates

### DS1307

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define RTC_DS1307

#define PIN_SCL 1 // I²C SCL
#define PIN_SDA 2 // I²C SDA
```

> [!IMPORTANT]
> [Logic level shifter](#%EF%B8%8F-logic-level-shifter) required.

> [!WARNING]
> Incompatible with [IKEA Frekvens](https://github.com/VIPnytt/Frekvens/wiki/IKEA-Frekvens) due to the lack of a 5 V power supply.

### DS3231

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define RTC_DS3231

#define PIN_SCL 1 // I²C SCL
#define PIN_SDA 2 // I²C SDA
#define PIN_INT 3 // RTC INT
```

### DS3232

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define RTC_DS3232

#define PIN_SCL 1 // I²C SCL
#define PIN_SDA 2 // I²C SDA
#define PIN_INT 3 // RTC INT
```

### PCF8563

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define RTC_PCF8563

#define PIN_SCL 1 // I²C SCL
#define PIN_SDA 2 // I²C SDA
#define PIN_INT 3 // RTC INT
```
