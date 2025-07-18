import dotenv

from tools.src.config import Frekvens
from tools.src.config import Obegransad

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
    Frekvens.COLUMNS if ENV_FREKVENS else Obegransad.COLUMNS if ENV_OBEGRANSAD else None
)
MODEL = Frekvens.MODEL if ENV_FREKVENS else Obegransad.MODEL if ENV_OBEGRANSAD else None
ROWS = Frekvens.ROWS if ENV_FREKVENS else Obegransad.ROWS if ENV_OBEGRANSAD else None
