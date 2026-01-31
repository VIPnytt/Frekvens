# PlatformIO pre-build extra script

import os
import SCons.Script
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
        env.Execute(
            "pip install -r scripts/requirements.txt"
            if int(SCons.Script.ARGUMENTS["PIOVERBOSE"]) or "CI" in os.environ
            else "pip install -q -r scripts/requirements.txt"
        )

    sys.path.append(env["PROJECT_DIR"])

    from scripts.src.Frekvens import Frekvens  # noqa: E402

    Frekvens.clean() if env.IsCleanTarget() else Frekvens(env).run()
