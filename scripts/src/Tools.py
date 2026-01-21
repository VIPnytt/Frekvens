import os
import shutil
import typing

from .config.version import VERSION

if typing.TYPE_CHECKING:
    from .Frekvens import Frekvens


class Tools:
    NAME: str = "Tools"
    project: "Frekvens"

    def __init__(self, project: "Frekvens") -> None:
        self.project = project

    def initialize(self) -> None:
        with open("tools/pyproject.toml", "r", encoding="utf-8") as toml:
            contents = toml.read()
            if f'version = "{VERSION}"' not in contents and f'version = "{VERSION.replace("-", ".")}"' not in contents:
                raise ValueError(f"{self.NAME} version mismatch")

    @staticmethod
    def clean() -> None:
        for path in [
            "tools/__pycache__",
        ]:
            if os.path.exists(path):
                shutil.rmtree(path, ignore_errors=True)
                print(f"Removing {path}")
