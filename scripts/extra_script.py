# PlatformIO pre-build extra script

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
        command = "uv sync --only-group scripts --inexact"
        if int(SCons.Script.ARGUMENTS["PIOVERBOSE"]):
            command += " --verbose"
        if env.Execute(command):
            env.Execute(f"pip install uv && {command}")

    sys.path.append(env["PROJECT_DIR"])

    from scripts.src.Frekvens import Frekvens  # noqa: E402

    Frekvens.clean() if env.IsCleanTarget() else Frekvens(env).run()
