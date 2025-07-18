# ✨ Custom devices

Even if Frekvens was designed for [IKEA Frekvens](Frekvens) and [IKEA Obegränsad](Obegransad), which both are [SCT2024](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf)-based, the firmware can easily be adapted for custom or third-party devices, including those with different display resolutions or configurations, with only minor adjustments.

## 💻 Resolution

Most [modes](Modes) are designed for flexible display resolutions, the easiest way to getting started is to re-use the config of an existing device, eg. [IKEA Frekvens]([..](https://github.com/VIPnytt/Frekvens)/blob/main/firmware/include/config/Frekvens.h) or [IKEA Obegränsad](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/Obegransad.h), but what's different.

Example:

```h
#define COLUMNS 16     // px
#define ROWS 16        // px

#define PIXEL_ORDER {} // bit mask array
```

## 📱 Web app

The *Web app* user-interface will continue to be functional no matter what device is specified.

Even custom resolutions is supported for interactive [modes](Modes), as long as the `MODEL` is set to a known value.
