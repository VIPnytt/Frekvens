# ⛅ Weather

At least one weather provider is required to use the [Weather](https://github.com/VIPnytt/Frekvens/wiki/Modes#-weather) mode.

| Provider                                       | Open access             | Coordinates             | Named location          | No place config    |
| ---------------------------------------------- | ----------------------- | ----------------------- | ----------------------- | ------------------ |
| [Google](%EF%B8%8F-google)                     | :x:                     | :white_check_mark:      | :x:                     | :x:                |
| [Home Assistant](#-home-assistant)             | :x:                     | :x:                     | :x:                     | :white_check_mark: |
| [Open-Meteo](#-open-meteo)                     | :ballot_box_with_check: | :white_check_mark:      | :x:                     | :x:                |
| [Open Weather](#️-open-weather)                 | :x:                     | :white_check_mark:      | :white_check_mark:      | :x:                |
| [Tomorrow.io](#-tomorrowio)                    | :x:                     | :white_check_mark:      | :white_check_mark:      | :x:                |
| [World Weather Online](#-world-weather-online) | :x:                     | :white_check_mark:      | :x:                     | :x:                |
| [Wttr.in](#️-wttrin)                            | :white_check_mark:      | :white_check_mark:      | :white_check_mark:      | :white_check_mark: |
| [Yr](#️-yr)                                     | :white_check_mark:      | :white_check_mark:      | :x:                     | :x:                |

## ☁️ Google

Refreshed every ~17 minutes.

Requires a [API-key](https://developers.google.com/maps/documentation/weather/get-api-key). Consumes up to 2 555 requests/month, depending on usage.

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define GOOGLEWEATHER_KEY "secret"
```

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_WEATHER='true'
WEATHER_GOOGLE='true'
```

> [!NOTE]
> Location coordinates, `LATITUDE` and `LONGITUDE` are required.

See also [Weather](https://github.com/VIPnytt/Frekvens/wiki/Modes#-weather) mode.

## 🏡 Home Assistant

Smart-home integration.

Refreshed every ~4 minutes.

Requires an [Long-lived access token](https://my.home-assistant.io/redirect/profile_security/).

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define HOMEASSISTANT_KEY "secret"
```

```h
#define HOMEASSISTANT_PROTOCOL "http:"               // optional
#define HOMEASSISTANT_HOST "homeassistant.local"
#define HOMEASSISTANT_PORT 8123                      // optional
#define HOMEASSISTANT_ENTITY "weather.forecast_home" // optional
```

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_WEATHER='true'
WEATHER_HOMEASSISTANT='true'
```

See also [Home Assistant](https://github.com/VIPnytt/Frekvens/wiki/Extensions#home-assistant) extension and [Weather](https://github.com/VIPnytt/Frekvens/wiki/Modes#-weather) mode.

## ⛅ Open-Meteo

Refreshed every ~17 minutes.

Commercial usage requires an [API-key](https://open-meteo.com/en/pricing). Consumes up to 2 555 requests/month, depending on usage.

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define OPENMETEO_KEY "secret" // optional
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

Refreshed every ~17 minutes.

Requires a [API-key](https://home.openweathermap.org/api_keys). Consumes up to 83 requests/day and 2 555 requests/month, depending on usage.

Order of precedence:

- `LATITUDE` and `LONGITUDE`
- `LOCATION`

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

## 🌀 Tomorrow.io

Refreshed every ~17 minutes.

Requires a [API-key](https://app.tomorrow.io/development/keys). Consumes up to 83 requests/day, depending on usage.

Order of precedence:

- `LATITUDE` and `LONGITUDE`
- `LOCATION`

Configure in [secrets.h](https://github.com/VIPnytt/Frekvens/blob/main/firmware/include/config/secrets.h):

```h
#define TOMORROWIO_KEY "secret"
```

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_WEATHER='true'
WEATHER_TOMORROWIO='true'
```

> [!NOTE]
> Location coordinates `LATITUDE` and `LONGITUDE`, or a location name `LOCATION` is required.

See also [Weather](https://github.com/VIPnytt/Frekvens/wiki/Modes#-weather) mode.

## 🌍 World Weather Online

Refreshed every ~17 minutes.

Order of precedence:

- `LATITUDE` and `LONGITUDE`
- `LOCATION`

Requires a [API-key](https://www.worldweatheronline.com/weather-api/my/). Consumes up to 83 requests/day, depending on usage.

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

Refreshed every ~17 minutes.

Order of precedence:

- `LATITUDE` and `LONGITUDE`
- `LOCATION`
- IP address

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_WEATHER='true'
WEATHER_WTTRIN='true'
```

See also [Weather](https://github.com/VIPnytt/Frekvens/wiki/Modes#-weather) mode.

## 🌧️ Yr

Refreshed every ~9 minutes.

Configure in [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env):

```ini
MODE_WEATHER='true'
WEATHER_YR='true'
```

> [!NOTE]
> Location coordinates, `LATITUDE` and `LONGITUDE` are required.

See also [Weather](https://github.com/VIPnytt/Frekvens/wiki/Modes#-weather) mode.
