import os
import pathlib
import re
import subprocess
import sys
import typing

if typing.TYPE_CHECKING:
    from .src.components.Types import COMMAND_LINE_TARGETS, Environment, Import  # noqa: E402
else:
    from SCons.Script import COMMAND_LINE_TARGETS, Environment, Import  # noqa: E402


def main() -> None:
    if COMMAND_LINE_TARGETS in [
        ["erase"],
        ["menuconfig"],
        ["size"],
    ]:
        return

    Import("env")
    env: Environment = typing.cast(Environment, globals()["env"])

    uv_env = os.environ.copy()
    uv_env["VIRTUAL_ENV"] = os.path.join(env.subst("$PROJECT_CORE_DIR"), "penv")
    uv = subprocess.run(
        [sys.executable, "-m", "uv", "sync", "--active", "--inexact", "--only-group", "scripts"],
        stderr=subprocess.DEVNULL,
        env=uv_env,
    )
    if uv.returncode:
        match = re.search(r'"(uv==\d+\.\d+\.\d+)"', pathlib.Path("pyproject.toml").read_text())
        pip = subprocess.run(
            [sys.executable, "-m", "pip", "install", match.group(1) if match else "uv"],
            stderr=subprocess.DEVNULL,
        )
        if pip.returncode:
            subprocess.run([sys.executable, "-m", "ensurepip"], check=True)
            subprocess.run(pip.args, check=True)
        subprocess.run(uv.args, env=uv_env, check=True)

    from src.Frekvens import Frekvens  # noqa: E402

    if env.IsCleanTarget():
        Frekvens.clean()
        return

    Frekvens(env).run()


if __name__ == "SCons.Script":
    main()
