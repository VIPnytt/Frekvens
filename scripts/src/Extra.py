import os
import shutil
import typing

if typing.TYPE_CHECKING:
    from .Project import Project


class Extra:
    project: "Project"

    def __init__(self, project: "Project") -> None:
        self.project = project

    @staticmethod
    def clean() -> None:
        for path in [
            "extra/Python/__pycache__",
        ]:
            if os.path.exists(path):
                shutil.rmtree(path, ignore_errors=True)
                print(f"Removing {path}")
