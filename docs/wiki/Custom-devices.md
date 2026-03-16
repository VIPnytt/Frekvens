# ✨ Custom devices

> [!NOTE]
> Even if Frekvens was designed for [IKEA Frekvens](https://github.com/VIPnytt/Frekvens/wiki/IKEA-Frekvens) and [IKEA Obegränsad](https://github.com/VIPnytt/Frekvens/wiki/IKEA-Obegransad), which both are [SCT2024](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf) based, the firmware can easily be adapted for custom or third-party devices, including those with different display resolutions or configurations.

## 💻 Resolution

Most [modes](https://github.com/VIPnytt/Frekvens/wiki/Modes) are designed for flexible display resolutions, the easiest way to getting started is to re-use the config of an existing device, eg. [IKEA Frekvens](https:/github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/IkeaFrekvens.h) or [IKEA Obegränsad](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/IkeaObegransad.h), but only change what’s different.

Example:

```h
#define GRID_COLUMNS 16 // px
#define GRID_ROWS 32    // px

#define LED_MAP {0x0, 0x1, 0x2, 0x3} // bit mask array
```
