# PlatformIO pre-build extra script

import re
import SCons.Script
import subprocess
import sys
import typing


if SCons.Script.COMMAND_LINE_TARGETS not in [
    ["erase"],
    ["menuconfig"],
    ["size"],
]:
    if typing.TYPE_CHECKING:

        def Import(*vars) -> None:
            pass

    Import("env")

    if typing.TYPE_CHECKING:
        env = SCons.Script.Environment()

    if not env.IsCleanTarget():
        uv = subprocess.run(
            [sys.executable, "-m", "uv", "sync", "--active", "--inexact", "--only-group", "scripts"],
            stderr=subprocess.DEVNULL,
        )
        if uv.returncode:
            with open("pyproject.toml", encoding="utf-8") as f:
                match = re.search(r'"(?P<package>uv==\d+\.\d+\.\d+)"', f.read())
            pip = subprocess.run(
                [sys.executable, "-m", "pip", "install", match["package"] if match else "uv"],
                stderr=subprocess.DEVNULL,
            )
            if pip.returncode:
                subprocess.run([sys.executable, "-m", "ensurepip"], check=True)
                subprocess.run(pip.args, check=True)
            subprocess.run(uv.args, check=True)

    sys.path.append(env["PROJECT_DIR"])

    from scripts.src.Frekvens import Frekvens  # noqa: E402

    Frekvens.clean() if env.IsCleanTarget() else Frekvens(env).run()
