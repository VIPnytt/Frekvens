# Pre-build script

import sys

Import("env")  # type: ignore
sys.path.insert(0, env["PROJECT_DIR"])  # type: ignore

if not env.IsCleanTarget():  # type: ignore
    env.Execute(f"pip -q install -r scripts/requirements.txt")  # type: ignore
    env.Execute(f"pip -q install tools/")  # type: ignore

from scripts import Firmware, Scripts, Tools, WebApp

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
    ["menuconfig"],
    ["uploadfs"],
    ["uploadfsota"],
]:
    firmware = Firmware.Firmware(env)  # type: ignore
    firmware.version()
    firmware.define()
    firmware.upload()

if not env.IsCleanTarget() and COMMAND_LINE_TARGETS not in [  # type: ignore
    ["build"],
    ["erase"],
    ["menuconfig"],
    ["size"],
    ["upload"],
]:
    webapp = WebApp.WebApp(env)  # type: ignore
    if webapp.check():
        webapp.version()
        webapp.evironment()
        webapp.environment_dev()
        webapp.build()

if not env.IsCleanTarget() and COMMAND_LINE_TARGETS not in [  # type: ignore
    ["erase"],
    ["menuconfig"],
    ["size"],
]:
    tools = Tools.Tools(env)  # type: ignore
    tools.version()
    tools.environment()

if env.IsCleanTarget():  # type: ignore
    tools = Tools.Tools(env)  # type: ignore
    tools.clean()
    webapp = WebApp.WebApp(env)  # type: ignore
    webapp.clean()
    scripts = Scripts.Scripts(env)  # type: ignore
    scripts.clean()

# Dump environment variables
# print(env.Dump())# type: ignore
