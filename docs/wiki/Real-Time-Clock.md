# ⏰ Real-Time Clock

A Real-Time Clock (RTC) module keeps accurate time, even without internet access.

Most common RTC modules will work. Good starting points include the [DS3231](https://www.adafruit.com/product/3013) and the [DS1307](https://www.adafruit.com/product/3296).

## ✅ Supported types

- [DS1302](https://www.parallax.com/product/ds1302-real-time-clock-module/)
- [DS1307](https://www.adafruit.com/product/3296)
- [DS3231](https://www.adafruit.com/product/3013)
- [DS3232](https://www.dfrobot.com/product-2304.html)
- [DS3234](https://www.sparkfun.com/sparkfun-deadon-rtc-breakout-ds3234.html)
- [PCF8563](https://www.waveshare.com/PCF8563-RTC-Board.htm)

> [DS1307](#ds1307) operates at 5 V and is therefore incompatible with [IKEA Frekvens](https://github.com/VIPnytt/Frekvens/wiki/IKEA-Frekvens). For usage with other devices, an [logic level shifter](#%EF%B8%8F-logic-level-shifter) is required.

## 📌 Schematics

### DS1302 schema

```text
┌──────┐
│  VCC ├─ +3.3 V DC
│  GND ├─ 0 V DC
|      |
│ SCLK ├─ SPI SCLK
│ SDIO ├─ SPI SDIO
│   CS ├─ SPI CS
└──────┘
```

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

### DS3234 schema

```text
┌──────┐
│  VCC ├─ +3.3 V DC
│  GND ├─ 0 V DC
|      |
│ SCLK ├─ SPI SCLK
│ MISO ├─ SPI MISO
│ MOSI ├─ SPI MOSI
│   CS ├─ SPI CS
|      |
│  INT ├─ RTC INT
└──────┘
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
│           SCLK ├─ SPI SCLK
│           MISO ├─ SPI MISO
│           MOSI ├─ SPI MOSI
│                │
│           MISO ├─ SPI SDIO
│                │
│ Digital output ├─ SPI CS
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
           │             │
SPI SCLK  ─┤     ──►     ├─ SPI SCLK
SPI MISO  ─┤     ◄──     ├─ SPI MISO
SPI MOSI  ─┤     ──►     ├─ SPI MOSI
  SPI CS  ─┤     ──►     ├─ SPI CS
           │             │
SPI SDIO  ─┤     ◄─►     ├─ SPI SDIO
           │             │
 RTC INT  ─┤     ◄──     ├─ RTC INT
           └─────────────┘
```

## ↔️ Logic level shifter

Most RTC modules are 3.3 V compatible, but some variants use higher logic levels (e.g. 5 V). In those cases, a logic level shifter is required to ensure safe communication with the ESP32. Since RTC modules can use either SPI or I²C depending on the variant, the level shifter should be suitable for the chosen interface.

## 🔧 Configuration

### I²C SCL

Serial clock for I²C communication.

Any I²C `SCL` pin can be used.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_SCL 1 // I²C SCL
```

### I²C SDA

Bidirectional data line for I²C.

Any I²C `SDA` pin can be used.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_SDA 2 // I²C SDA
```

### SPI SCLK

Serial clock for SPI communication.

Any SPI `SCLK` pin can be used.

> On boards with two SPI interfaces, either the `HSPI` or `VSPI` bus can be used. However, you must choose the one that is not connected to the display, as these devices cannot share the same SPI bus.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_SCLK2 3 // SPI SCLK
```

### SPI MOSI

Master-out data line for SPI.

Any SPI `MOSI` pin can be used.

> On boards with two SPI interfaces, either the `HSPI` or `VSPI` bus can be used. However, you must choose the one that is not connected to the display, as these devices cannot share the same SPI bus.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_MOSI2 4 // SPI MOSI
```

### SPI MISO

Master-in data line for SPI.

Any SPI `MISO` pin can be used.

> On boards with two SPI interfaces, either the `HSPI` or `VSPI` bus can be used. However, you must choose the one that is not connected to the display, as these devices cannot share the same SPI bus.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_MISO2 5 // SPI MISO
```

### SPI SDIO

Bidirectional data line for SPI.

Any SPI `MISO` pin can be used.

> On boards with two SPI interfaces, either the `HSPI` or `VSPI` bus can be used. However, you must choose the one that is not connected to the display, as these devices cannot share the same SPI bus.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_SDIO2 6 // SPI SDIO
```

### SPI CS

Chip Select for the RTC module.

Any digital output pin can be used.

> Avoid strapping pins as this pin is pulled *LOW* using a resistor. On ESP32 (LX6-based) boards, it is recommended to use specialized pins, such as `CS` (often labeled `SS` on older boards).

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_CS2 7 // SPI CS
```

### RTC INT

Interrupt signal from the RTC module.

Optional to connect.

Any digital input pin that are also RTC-capable can be used.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_INT 8 // RTC INT
```

## 🧩 Extension

Using the [RTC](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-rtc) extension, the clock will automatically sync during during startup.

Check out the [RTC](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-rtc) extension for more info.

## 📝 Templates

### DS1302

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define RTC_DS1302

#define PIN_SCLK2 1 // SPI SCLK
#define PIN_SDIO2 2 // SPI SDIO
#define PIN_CS2 3   // SPI CS
```

> Make sure the ESP32 of choice supports both *HSPI* and *VSPI*, as one of them needs to be dedicated to the display.

### DS1307

[Logic level shifter](#%EF%B8%8F-logic-level-shifter) required.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define RTC_DS1307

#define PIN_SCL 1 // I²C SCL
#define PIN_SDA 2 // I²C SDA
```

> Incompatible with [IKEA Frekvens](https://github.com/VIPnytt/Frekvens/wiki/IKEA-Frekvens) due to the lack of a 5 V power supply.

### DS3231

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define RTC_DS3231

#define PIN_SCL 1 // I²C SCL
#define PIN_SDA 2 // I²C SDA
#define PIN_INT 3 // RTC INT
```

### DS3232

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define RTC_DS3232

#define PIN_SCL 1 // I²C SCL
#define PIN_SDA 2 // I²C SDA
#define PIN_INT 3 // RTC INT
```

### DS3234

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define RTC_DS3234

#define PIN_SCLK2 1 // SPI SCLK
#define PIN_MISO2 2 // SPI MISO
#define PIN_MOSI2 3 // SPI MOSI
#define PIN_CS2 4   // SPI CS
#define PIN_INT 5   // RTC INT
```

> Make sure the ESP32 of choice supports both *HSPI* and *VSPI*, as one of them needs to be dedicated to the display.

### PCF8563

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define RTC_PCF8563

#define PIN_SCL 1 // I²C SCL
#define PIN_SDA 2 // I²C SDA
#define PIN_INT 3 // RTC INT
```
