# ✨ Custom devices

> Even if Frekvens was designed for [IKEA Frekvens](https://github.com/VIPnytt/Frekvens/wiki/IKEA-Frekvens) and [IKEA Obegränsad](https://github.com/VIPnytt/Frekvens/wiki/IKEA-Obegransad), which both are [SCT2024](http://www.starchips.com.tw/pdf/datasheet/SCT2024V01_03.pdf)-based, the firmware can easily be adapted for custom or third-party devices, including those with different display resolutions or configurations.

## 💻 Resolution

Most [modes](https://github.com/VIPnytt/Frekvens/wiki/Modes) are designed for flexible display resolutions, the easiest way to getting started is to re-use the config of an existing device, eg. [IKEA Frekvens](https:/github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/Frekvens.h) or [IKEA Obegränsad](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/Obegransad.h), but only change what's different.

Example:

```h
#define COLUMNS 16 // px
#define ROWS 32    // px

#define PIXEL_ORDER {0x0, 0x1, 0x2, 0x3} // bit mask array
```

## 📱 Web app

The [Web app](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-web-app) UI will continue to be functional no matter what device is specified.

Even custom resolutions is supported for interactive [modes](https://github.com/VIPnytt/Frekvens/wiki/Modes), as long as the `MODEL` is set to a supported value.
