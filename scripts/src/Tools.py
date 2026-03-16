import os
import shutil
import typing

if typing.TYPE_CHECKING:
    from .Frekvens import Frekvens


class Tools:
    NAME: str = "Tools"
    project: "Frekvens"

    def __init__(self, project: "Frekvens") -> None:
        self.project = project

    @staticmethod
    def clean() -> None:
        for path in [
            "tools/__pycache__",
        ]:
            if os.path.exists(path):
                shutil.rmtree(path, ignore_errors=True)
                print(f"Removing {path}")
