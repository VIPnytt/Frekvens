# ⏰ Real-Time Clock

An RTC-module can provide a reliable clock, even without Wi-Fi connectivity.

## ✅ Supported types

- [DS1302](https://www.parallax.com/product/ds1302-real-time-clock-module/)
- [DS1307](https://www.adafruit.com/product/3296)
- [DS3231](https://www.adafruit.com/product/3013)
- [DS3232](https://www.dfrobot.com/product-2304.html)
- [DS3234](https://www.sparkfun.com/sparkfun-deadon-rtc-breakout-ds3234.html)
- [PCF8563](https://www.waveshare.com/PCF8563-RTC-Board.htm)

> [DS1307](#ds1307) operates at 5 V and is therefore incompatible with [IKEA Frekvens](https://github.com/VIPnytt/Frekvens/wiki/IKEA-Frekvens). For usage with other devices, an [logic level shifter](#%EF%B8%8F-logic-level-shifter) is required.

## 📈 Schematics

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
│ SCL ├─ I2C SCL
│ SDA ├─ I2C SDA
└─────┘
```

### DS3231 schema

```text
┌─────┐
│ VCC ├─ +3.3 V DC
│ GND ├─ 0 V DC
|     |
│ SCL ├─ I2C SCL
│ SDA ├─ I2C SDA
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
│ SCL ├─ I2C SCL
│ SDA ├─ I2C SDA
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
│ SCL ├─ I2C SCL
│ SDA ├─ I2C SDA
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
│        I2C SCL ├─ I2C SCL
│        I2C SDA ├─ I2C SDA
│                │
│       SPI SCLK ├─ SPI SCLK
│       SPI MISO ├─ SPI MISO
│       SPI MOSI ├─ SPI MOSI
│                │
│       SPI MISO ├─ SPI SDIO
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
 I2C SCL  ─┤             ├─ I2C SCL
 I2C SDA  ─┤             ├─ I2C SDA
           │             │
SPI SCLK  ─┤             ├─ SPI SCLK
SPI MISO  ─┤             ├─ SPI MISO
SPI MOSI  ─┤             ├─ SPI MOSI
  SPI CS  ─┤             ├─ SPI CS
           │             │
SPI SDIO  ─┤             ├─ SPI SDIO
           │             │
 RTC INT  ─┤             ├─ RTC INT
           └─────────────┘
```

## ↔️ Logic level shifter

Components connected to voltage levels above 3.3 V often requires an voltage-level translator to remain ESP32-compatible.
Most RTC-modules are 3.3 V compatible, but if 5 V logic is desired, the easiest solution might be to use an [TXS0108E](https://www.sparkfun.com/sparkfun-level-shifter-8-channel-txs0108e.html) which is both SPI and I2C compatible. For SPI specifically it's also possible to use [TXB0104](https://www.adafruit.com/product/1875) or [TXB0108](https://www.adafruit.com/product/395), but be aware that these does not work as well with strong pull-up or pull-down resistors *some* RTC-modules *may* have on the `INT` pin.

## 🔧 Configuration

### I2C SCL

Any *I2C `SCL`* pin can be used.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_SCL 1 // GPIO #
```

### I2C SDA

Any *I2C `SDA`* pin can be used.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_SDA 2 // GPIO #
```

### SPI SCLK

Any *SPI `SCLK`* pin can be used.

> On boards with two SPI interfaces, either the `HSPI` or `VSPI` bus can be used. However, you must choose the one that is not connected to the display, as these devices cannot share the same SPI bus.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_SCLK2 3 // GPIO #
```

### SPI MISO

Any *SPI `MISO`* pin can be used.

> On boards with two SPI interfaces, either the `HSPI` or `VSPI` bus can be used. However, you must choose the one that is not connected to the display, as these devices cannot share the same SPI bus.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_MISO2 4 // GPIO #
```

### SPI MOSI

Any *SPI `MOSI`* pin can be used.

> On boards with two SPI interfaces, either the `HSPI` or `VSPI` bus can be used. However, you must choose the one that is not connected to the display, as these devices cannot share the same SPI bus.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_MOSI2 5 // GPIO #
```

### SPI SDIO

Any *SPI `MISO`* pin can be used.

> On boards with two SPI interfaces, either the `HSPI` or `VSPI` bus can be used. However, you must choose the one that is not connected to the display, as these devices cannot share the same SPI bus.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_SDIO2 6 // GPIO #
```

### SPI CS

Any *digital output* pin can be used.

> Avoid strapping pins as this pin is pulled *LOW* using a resistor. On ESP32 (LX6-based, original series) boards, it is recommended to use specialized pins, such as `CS` (often labeled `SS` on older boards).

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_CS2 7 // GPIO #
```

### RTC INT

Optional to connect.

Any *Digital input* pin that are also RTC-capable can be used.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define PIN_INT 8 // GPIO #
```

## 🧩 Extension

Using the [RTC](Extensions#-rtc) extension, the clock will automatically sync during during startup.

Check out the [RTC](Extensions#-rtc) extension for more info.

## 📝 Templates

### DS1302

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define RTC_DS1302

#define PIN_SCLK2 1
#define PIN_SDIO2 2
#define PIN_CS2 3
```

> Make sure the ESP32 of choice supports both *HSPI* and *VSPI*, as one of them needs to be dedicated to the display.

### DS1307

Logic level shifter required.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define RTC_DS1307

#define PIN_SCL 1
#define PIN_SDA 2
```

> Incompatible with [IKEA Frekvens](IKEA-Frekvens) due to the lack of a 5 V power supply.

### DS3231

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define RTC_DS3231

#define PIN_SCL 1
#define PIN_SDA 2
#define PIN_INT 3
```

### DS3232

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define RTC_DS3232

#define PIN_SCL 1
#define PIN_SDA 2
#define PIN_INT 3
```

### DS3234

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define RTC_DS3234

#define PIN_SCLK2 1
#define PIN_MISO2 2
#define PIN_MOSI2 3
#define PIN_CS2 4
#define PIN_INT 5
```

> Make sure the ESP32 of choice supports both *HSPI* and *VSPI*, as one of them needs to be dedicated to the display.

### PCF8563

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define RTC_PCF8563

#define PIN_SCL 1
#define PIN_SDA 2
#define PIN_INT 3
```
