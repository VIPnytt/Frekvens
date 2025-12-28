# PlatformIO pre-build extra script

import sys

Import("env")  # type: ignore

if not env.IsCleanTarget():  # type: ignore
    env.Execute(f"pip install -r scripts/requirements.txt -q")  # type: ignore

sys.path.append(env["PROJECT_DIR"])  # type: ignore

from scripts.src.Frekvens import Frekvens

if not env.IsCleanTarget() and COMMAND_LINE_TARGETS not in [  # type: ignore
    ["erase"],
    ["menuconfig"],
    ["size"],
]:
    Frekvens(env, COMMAND_LINE_TARGETS).run()  # type: ignore

if env.IsCleanTarget():  # type: ignore
    Frekvens.clean()
