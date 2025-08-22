import dotenv

from . import ikeaFrekvens, ikeaObegransad

ENV_FREKVENS = (
    True if "ENV_FREKVENS" in dotenv.dotenv_values("tools/.env").keys() else False
)
ENV_OBEGRANSAD = (
    True if "ENV_OBEGRANSAD" in dotenv.dotenv_values("tools/.env").keys() else False
)

HOST = (dotenv.dotenv_values("tools/.env").get("HOSTNAME") or "frekvens") + (
    dotenv.dotenv_values("tools/.env").get("DOMAIN") or ".local"
)

COLUMNS = (
    ikeaFrekvens.COLUMNS
    if ENV_FREKVENS
    else ikeaObegransad.COLUMNS if ENV_OBEGRANSAD else 0
)
ROWS = (
    ikeaFrekvens.ROWS if ENV_FREKVENS else ikeaObegransad.ROWS if ENV_OBEGRANSAD else 0
)
MODEL = (
    ikeaFrekvens.MODEL
    if ENV_FREKVENS
    else ikeaObegransad.MODEL if ENV_OBEGRANSAD else None
)

MODE_ARTNET = (
    True if dotenv.dotenv_values("tools/.env").get("MODE_ARTNET") != "false" else False
)
MODE_DISTRIBUTEDDISPLAYPROTOCOL = (
    True
    if dotenv.dotenv_values("tools/.env").get("MODE_DISTRIBUTEDDISPLAYPROTOCOL")
    != "false"
    else False
)
MODE_E131 = (
    True if dotenv.dotenv_values("tools/.env").get("MODE_E131") != "false" else False
)
