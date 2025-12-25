# PlatformIO pre-build extra script

import logging
import sys

Import("env")  # type: ignore

if not env.IsCleanTarget():  # type: ignore
    env.Execute(f"pip install -r scripts/requirements.txt -q")  # type: ignore

sys.path.append(env["PROJECT_DIR"])  # type: ignore

from scripts.src.Frekvens import Frekvens

# Dump CLI targets
# print(COMMAND_LINE_TARGETS)  # type: ignore

if env["PIOENV"] in [  # type: ignore
    "upload_ota",
]:
    logging.info(f"Skipping environment: {env['PIOENV']}")  # type: ignore
    env.Exit(0)  # type: ignore

if not env.IsCleanTarget() and COMMAND_LINE_TARGETS not in [  # type: ignore
    ["erase"],
    ["menuconfig"],
    ["size"],
]:
    Frekvens(env, COMMAND_LINE_TARGETS).run()  # type: ignore

if env.IsCleanTarget():  # type: ignore
    Frekvens.clean()

# Dump environment variables
# print(env.Dump())  # type: ignore
