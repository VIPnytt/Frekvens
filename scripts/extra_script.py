import os
import subprocess
import sys
import typing

if typing.TYPE_CHECKING:
    from .src.components.Types import COMMAND_LINE_TARGETS, Environment, Import
else:
    from SCons.Script import COMMAND_LINE_TARGETS, Environment, Import


def main() -> None:
    """
    Run the Frekvens build workflow for the current SCons environment.

    The function synchronizes the scripts dependencies, cleans the project for
    clean targets, and otherwise executes the Frekvens workflow. It returns
    without action for the ``erase``, ``menuconfig``, and ``size`` targets.
    """
    if not {
        "erase",
        "menuconfig",
        "size",
    }.isdisjoint(COMMAND_LINE_TARGETS):
        return

    Import("env")
    env: Environment = typing.cast(Environment, globals()["env"])

    uv_env = os.environ.copy()
    uv_env["VIRTUAL_ENV"] = os.path.join(env.subst("$PROJECT_CORE_DIR"), "penv")
    uv = subprocess.run(
        [sys.executable, "-m", "uv", "sync", "--active", "--inexact", "--only-group", "scripts"],
        stderr=subprocess.DEVNULL,
        env=uv_env,
        check=False,
    )
    if uv.returncode:
        pip = subprocess.run(
            [sys.executable, "-m", "pip", "install", "uv"],
            stderr=subprocess.DEVNULL,
            check=False,
        )
        if pip.returncode:
            subprocess.run([sys.executable, "-m", "ensurepip"], check=True)
            subprocess.run(pip.args, check=True)
        subprocess.run(uv.args, env=uv_env, check=True)

    from src.Frekvens import Frekvens

    if env.IsCleanTarget():
        Frekvens.clean()
        return

    Frekvens(env).run()


if __name__ == "SCons.Script":
    main()
