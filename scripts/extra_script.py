# PlatformIO pre-build extra script

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
        dependencies = subprocess.run(
            [sys.executable, "-m", "uv", "sync", "--active", "--inexact", "--only-group", "scripts"],
            stderr=subprocess.DEVNULL,
        )
        if dependencies.returncode:
            subprocess.run([sys.executable, "-m", "pip", "install", ".[bootstrap]"], check=True)
            subprocess.run(dependencies.args, check=True)

    sys.path.append(env["PROJECT_DIR"])

    from scripts.src.Frekvens import Frekvens  # noqa: E402

    Frekvens.clean() if env.IsCleanTarget() else Frekvens(env).run()
