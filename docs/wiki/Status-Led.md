# 🟢 Status LED

Provides low-level visual feedback of the device state. Primarily intended for basic troubleshooting when the device does not behave as expected.

The typical use case is the onboard LED commonly found on ESP32 boards, but any GPIO-connected LED can be used.

> [!NOTE]
> Some boards use an inverted onboard LED, meaning it lights up when turned off in software, and turns off when set to on.

## 🔧 Configuration

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define PIN_LED 1
```

## 🧩 Extension

With the [Status LED](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-status-led) extension enabled, the LED reflects device state:

- Fast blink — error condition
- Slow blink — warning or connectivity issue
- Solid on — idle/normal operation
