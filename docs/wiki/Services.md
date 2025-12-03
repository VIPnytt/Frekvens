# ⚙️ Services

[Connectivity](#-connectivity) | [Device](#️-device) | [Display](#-display) | [Modes](#️-modes) | [Web server](#️-web-server)

## 🌐 Connectivity

**Hostname:**

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
HOSTNAME='frekvens'
```

**Time zone:**

Time zone in IANA format, eg. `America/New_York`, `Asia/Shanghai` or `Europe/London`.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
TIME_ZONE='Etc/Universal'
```

**Wi-Fi client:**

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define WIFI_SSID "name"
#define WIFI_KEY "secret"
```

> [!TIP]
> Additional Wi-Fi credentials can be added later via the [Web app](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-web-app).

**Wi-Fi hotspot:**

Long press *any* [button](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-button) during startup to activate the [Wi-Fi hotspot](https://github.com/VIPnytt/Frekvens/wiki/Services#-connectivity). The [Web app](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-web-app) can then be used to add new Wi-Fi credentials if needed.

**Wi-Fi country:**

Regulatory country code in *ISO 3166-1 alpha-2* format.

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define WIFI_COUNTRY "01"
```

> [!NOTE]
> Only a subset of country codes are [supported by Espressif](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_wifi.html#_CPPv425esp_wifi_set_country_codePKcb).

## 🖥️ Device

**Name:**

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

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

Performs a full reset of the device’s non-volatile storage (NVS), clearing all user preferences, settings, and extra Wi-Fi credentials.
This is functionally equivalent to a *factory reset*.

API payload example:

```json
{
    "action": "restore"
}
```

> [!NOTE]
> The device will shut down automatically. A manual power cycle is required to power it back on again.

## 📺 Display

**Frame rate:**

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define FRAME_RATE 60 // fps
```

## 🎛️ Modes

**Weather:**

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define LATITUDE "0.000"  // coordinate
#define LONGITUDE "0.000" // coordinate
```

> [!NOTE]
> Some providers have resolution grid sizes down to 250 m (820 ft), which means 3-4 decimals should be provided for the most accurate weather reports.

A small list of providers also supports location, usually in the form of a city or village.

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define LOCATION "city"
```

Some also require an temperature unit.

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define TEMPERATURE_CELSIUS true // °C
```

```h
#define TEMPERATURE_FAHRENHEIT true // °F
```

```h
#define TEMPERATURE_KELVIN true // °K
```
