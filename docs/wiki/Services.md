# ⚙️ Services

## 🖥️ Device

### 🆔 Name

Set the device name using `NAME`.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
NAME='Frekvens'
```

API examples:

```json
{
    "action": "sleep"
}
```

```json
{
    "action": "reboot"
}
```

## 📺 Display

### 🎬 Frame rate

The default frame rate is 60 fps for most ESP32 variants (240 MHz). Depending on what [extensions](Extensions.md) and [modes](Modes.md) are included in the build, it's possible to increase the frame rate drastically.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define FRAME_RATE 60 // fps
```

## 🌐 Network

### 🔗 Host

Hostname is automatically set based on device type.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
HOSTNAME='frekvens'
DOMAIN='.local'
```

### 🗺️ DNS

DHCP provides up to two DNS servers, but it's actually possible to set up three manually if desired.

The `DNS3` option is only possible to set manually.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define DNS1 "1.1.1.1" // IPv4
#define DNS2 "8.8.8.8" // IPv4
#define DNS3 "9.9.9.9" // IPv4
```

## 📜 Serial

### 📋 Log level

Useful when debugging or testing experimenting with the APIs.

[platformio.ini](https://github.com/VIPnytt/Frekvens/blob/main/platformio.ini) example:

```ini
build_flags =
    -DF_INFO
;   -DF_DEBUG
;   -DF_VERBOSE
```

### 💻 Monitor speed

Requires `monitor_speed` to be set.

[platformio.ini](https://github.com/VIPnytt/Frekvens/blob/main/platformio.ini) example:

```ini
monitor_speed = 115200 ; bits
```

## ✅ Tasks

### 🧩 Extensions

Too many [extensions](#-extensions) or API-requests simultaneously can cause memory overflow.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define TASK_STACK_EXTENSIONS 4096 // bytes
```

### 🎛️ Modes

Memory overflow can cause the ESP32 to crash.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define TASK_STACK_MODES 8192 // bytes
```

## 🕰️ Time

### 🌐 Time server

To be able to sync the clock over Wi-Fi, an NTP-server is required.

Check out the [server suggestion list](https://gist.github.com/mutin-sa/eea1c396b1e610a2da1e5550d94b0453), if unsure.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define NTP1 "pool.ntp.org"
#define NTP2 "time.nist.gov"
#define NTP3 "time.cloudflare.com"
```

### 🌍 Time zone

Time zone in IANA format, eg. `America/New_York`, `Asia/Shanghai` or `Europe/Istanbul`.

Lookup your [time zone](https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv) if unsure.

[.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) example:

```ini
TIME_ZONE_IANA='Etc/Universal'
```

It can also be provided in *POSIX* format, if desired.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define TIME_ZONE_POSIX "UTC0"
```

## ☀️ Weather

### 📍 Coordinates

Weather providers uses coordinates to derermine correct location.

Some providers have resolution grid sizes down to 250 m, which means at least 3 decimals have to be provided for the most accurate weather reports. At the same time, more than 4 decimals is not advised for privacy reasons. Check out [Accuracy of decimal places in Latitude and Longitude degrees](https://support.garmin.com/en-US/?faq=hRMBoCTy5a7HqVkxukhHd8) for more information.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define LATITUDE "0.000"  // coordinate
#define LONGITUDE "0.000" // coordinate
```

### 🏙️ Location

A small list of providers also supports location, usually in the form of a city or village.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define LOCATION "city"
```

## 📡 Wi-Fi

### 📶 Wi-Fi client

While Wi-Fi can be re-configured at any time, it's often less hassle to configure everything at once.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define WIFI_SSID "name"
#define WIFI_KEY "[REDACTED]"
```

### 📱 Wi-Fi hotspot

If no hotspot credentials is configured, an open hotspot will be configured automatically.

The hotspot will be activated automatically if no Wi-Fi credentials is configured, but can also be triggered by pressing and holding any physical button during startup. For security reasons, it won't activate if the Wi-Fi signal is lost or out of reach.

[secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h) example:

```h
#define WIFI_SSID_HOTSPOT "name"
#define WIFI_KEY_HOTSPOT "[REDACTED]"
```

See also [Button](Extensions.md#%EF%B8%8F-button) extension.
