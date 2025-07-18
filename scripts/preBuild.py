# Pre-build script

import subprocess
import sys

Import("env")  # type: ignore
sys.path.insert(0, env["PROJECT_DIR"])  # type: ignore
subprocess.check_call(["pip", "install", "--quiet", "-r", "tools/requirements.txt"])

from tools.src.Firmware import Firmware
from tools.src.Tools import Tools
from tools.src.WebApp import WebApp

# Dump CLI targets
# print(COMMAND_LINE_TARGETS)# type: ignore

if env["PIOENV"] in [  # type: ignore
    "upload_ota",
    "upload_ota_migration",
]:
    print(f"Skipping environment: {env['PIOENV']}")  # type: ignore
    env.Exit(0)  # type: ignore

if not env.IsCleanTarget() and COMMAND_LINE_TARGETS not in [  # type: ignore
    ["buildfs"],
    ["erase"],
    ["uploadfs"],
    ["uploadfsota"],
]:
    firmware = Firmware(env)  # type: ignore
    firmware.version()
    firmware.define()
    firmware.upload()

if not env.IsCleanTarget() and COMMAND_LINE_TARGETS not in [  # type: ignore
    ["erase"],
    ["size"],
    ["upload"],
]:
    webapp = WebApp(env)  # type: ignore
    webapp.version()
    webapp.evironment()
    webapp.environment_dev()
    webapp.build()
    webapp.dist()

if not env.IsCleanTarget() and COMMAND_LINE_TARGETS not in [  # type: ignore
    ["erase"],
    ["size"],
]:
    tools = Tools(env)  # type: ignore
    tools.version()
    tools.environment()

if env.IsCleanTarget():  # type: ignore
    webapp = WebApp(env)  # type: ignore
    webapp.clean()

# Dump environment variables
# print(env.Dump())# type: ignore
