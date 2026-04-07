# ⛅ Weather

Provider support may vary depending on location.

[Google](%EF%B8%8F-google) | [Home Assistant](#-home-assistant) | [Open-Meteo](#-open-meteo) | [Open Weather](#️-open-weather) | [World Weather Online](#-world-weather-online) | [Wttr.in](#️-wttrin) | [Yr](#️-yr)

## ☁️ Google

Updated about every ~17 minutes.

Requires a [API-key](https://developers.google.com/maps/documentation/weather/get-api-key).

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define GOOGLEWEATHER_KEY "secret"
```

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_WEATHER='true'
WEATHER_GOOGLEWEATHER='true'
```

> [!NOTE]
> Location coordinates, `LATITUDE` and `LONGITUDE` are required.

See also [Weather](https://github.com/VIPnytt/Frekvens/wiki/Modes#-weather) mode.

## 🏡 Home Assistant

Smart-home integration.

Updated about every ~4 minutes.

Requires an [Long-lived access token](https://my.home-assistant.io/redirect/profile_security/).

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define HOMEASSISTANT_KEY "secret"
```

```h
#define HOMEASSISTANT_PROTOCOL "http:"
#define HOMEASSISTANT_HOST "homeassistant.local"
#define HOMEASSISTANT_PORT 8123
#define HOMEASSISTANT_ENTITY "weather.forecast_home"
```

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_WEATHER='true'
WEATHER_HOMEASSISTANT='true'
```

See also [Home Assistant](https://github.com/VIPnytt/Frekvens/wiki/Extensions#home-assistant) extension and [Weather](https://github.com/VIPnytt/Frekvens/wiki/Modes#-weather) mode.

## ⛅ Open-Meteo

Updated about every ~17 minutes.

Commercial usage requires an [API-key](https://open-meteo.com/en/pricing).

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define OPENMETEO_KEY "secret"
```

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_WEATHER='true'
WEATHER_OPENMETEO='true'
```

> [!NOTE]
> Location coordinates, `LATITUDE` and `LONGITUDE` are required.

See also [Weather](https://github.com/VIPnytt/Frekvens/wiki/Modes#-weather) mode.

## ☀️ Open Weather

Updated about every ~17 minutes.

Requires a [API-key](https://openweathermap.org/api), set using the `OPENWEATHER_KEY` variable.

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define OPENWEATHER_KEY "secret"
```

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_WEATHER='true'
WEATHER_OPENWEATHER='true'
```

> [!NOTE]
> Location coordinates, `LATITUDE` and `LONGITUDE` are required.

See also [Weather](https://github.com/VIPnytt/Frekvens/wiki/Modes#-weather) mode.

## 🌍 World Weather Online

Updated about every ~17 minutes.

Order of precedence:

- `LATITUDE` and `LONGITUDE`.
- `LOCATION`

Requires a [API-key](https://www.worldweatheronline.com/weather-api/api/docs/), set using the `WORLDWEATHERONLINE_KEY` variable.

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define WORLDWEATHERONLINE_KEY "secret"
```

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_WEATHER='true'
WEATHER_WORLDWEATHERONLINE='true'
```

> [!NOTE]
> Location coordinates `LATITUDE` and `LONGITUDE`, or a location name `LOCATION` is required.

See also [Weather](https://github.com/VIPnytt/Frekvens/wiki/Modes#-weather) mode.

## ☀️ Wttr.in

Updated about every ~17 minutes.

Order of precedence:

- `LATITUDE` and `LONGITUDE`.
- `LOCATION`
- IP address

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_WEATHER='true'
WEATHER_WTTRIN='true'
```

See also [Weather](https://github.com/VIPnytt/Frekvens/wiki/Modes#-weather) mode.

## 🌧️ Yr

Updated about every ~9 minutes.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_WEATHER='true'
WEATHER_YR='true'
```

> [!NOTE]
> Location coordinates, `LATITUDE` and `LONGITUDE` are required.

See also [Weather](https://github.com/VIPnytt/Frekvens/wiki/Modes#-weather) mode.
