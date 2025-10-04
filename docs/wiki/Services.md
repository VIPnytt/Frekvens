# ⚙️ Services

## 🌐 Connectivity

**DNS:**

While `DNS1` and `DNS2` override any DHCP-provided servers, `DNS3` is only available programmatically.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define DNS1 "1.1.1.1" // IPv4
#define DNS2 "8.8.8.8" // IPv4
#define DNS3 "9.9.9.9" // IPv4
```

**Host:**

Hostname is by default set based on device type.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
HOSTNAME='frekvens'
DOMAIN='.local'
```

**Time server:**

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define NTP1 "pool.ntp.org"
#define NTP2 "time.nist.gov"
#define NTP3 "time.cloudflare.com"
```

**Time zone:**

Time zone in IANA format, eg. `America/New_York`, `Asia/Shanghai` or `Europe/Istanbul`.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
TIME_ZONE_IANA='Etc/Universal'
```

It can alternatively be provided in *POSIX* format.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define TIME_ZONE_POSIX "UTC0"
```

**Wi-Fi client:**

While Wi-Fi can be re-configured at any time, it’s often less hassle to configure everything at once.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define WIFI_SSID "name"
#define WIFI_KEY "secret"
```

**Wi-Fi hotspot:**

If no hotspot credentials is configured, an open hotspot will be configured automatically.

The hotspot will be activated automatically if no Wi-Fi credentials is configured, but can also be triggered by pressing and holding any physical button during startup. For security reasons, it won’t activate if the Wi-Fi signal is lost or out of reach.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define WIFI_SSID_HOTSPOT "name"
#define WIFI_KEY_HOTSPOT "secret"
```

See also [Button](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-button) extension.

## ☁️ Web server

**Host check:**

- Method: `OPTIONS`
- URL: `http://example.local/canonical`

## 🖥️ Device

**Identify:**

API payload example:

```json
{
    "action": "identify"
}
```

**Log level:**

Useful when debugging or experimenting with the APIs.

[platformio.ini](https://github.com/VIPnytt/Frekvens/blob/main/platformio.ini) example:

```ini
build_flags =
    -DF_INFO
;   -DF_DEBUG
;   -DF_VERBOSE
```

Requires `monitor_speed` to be set.

[platformio.ini](https://github.com/VIPnytt/Frekvens/blob/main/platformio.ini) example:

```ini
monitor_speed = 115200 ; bits/s
```

**Name:**

Set the device name using `NAME`.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
NAME='Frekvens'
```

**Power off:**

API payload example:

```json
{
    "action": "power"
}
```

**Reboot:**

API payload example:

```json
{
    "action": "reboot"
}
```

**Restore:**

Performs a full reset of the device’s non-volatile storage (NVS), clearing all user preferences, settings, and Wi-Fi credentials.
This is functionally equivalent to a "factory reset" for this customized device.

After the NVS partition is erased, the device will shut down automatically.

API payload example:

```json
{
    "action": "restore"
}
```

> A manual power cycle is required to restart the device.

## 📺 Display

**Frame rate:**

The achievable frame rate depends on the SPI speed. In practice, factors such as wiring layout, wire type and quality, connection integrity, cable length, and shielding can all influence signal reliability, potentially requiring speed adjustments for optimal performance.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define FRAME_RATE 60 // fps
#define SPI_FREQUENCY 10000000 // Hz
```

## 🧩 Extensions

**Task stack:**

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define TASK_STACK_EXTENSIONS 4096 // bytes
```

## 🎛️ Modes

**Task stack:**

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define TASK_STACK_MODES 8192 // bytes
```

**Weather:**

Some providers have resolution grid sizes down to 250 m, which means at least 3 decimals have to be provided for the most accurate weather reports. At the same time, more than 4 decimals is not advised for privacy reasons. Check out [Accuracy of decimal places in Latitude and Longitude degrees](https://support.garmin.com/en-US/?faq=hRMBoCTy5a7HqVkxukhHd8) for more information.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define LATITUDE "0.000"  // coordinate
#define LONGITUDE "0.000" // coordinate
```

A small list of providers also supports location, usually in the form of a city or village.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define LOCATION "city"
```
