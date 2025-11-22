class OpenMeteo:
    ENV_OPTION: str = "MODE_OPENMETEO"
    HOST_WHITELIST: list[str] = [
        "api.open-meteo.com",
        "customer-api.open-meteo.com",
    ]
    NAME: str = "Open-Meteo"
