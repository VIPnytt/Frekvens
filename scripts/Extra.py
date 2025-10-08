import os
import shutil


class Extra:
    def __init__(self, env) -> None:
        self.env = env

    def clean(self) -> None:
        for path in [
            "extra/Python/__pycache__",
        ]:
            if os.path.exists(path):
                shutil.rmtree(path, ignore_errors=True)
                print(f"Removing {path}")
