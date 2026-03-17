import json
import packaging.version
import pathlib
import re
import tomllib

from ...scripts.src.config.version import VERSION


def package_json() -> str:
    path = pathlib.Path("webapp") / "package.json"
    return json.loads(path.read_text()).get("version")


def package_lock_json() -> str:
    path = pathlib.Path("webapp") / "package-lock.json"
    return json.loads(path.read_text())["packages"][""]["version"]


def pyproject_toml() -> str:
    path = pathlib.Path("pyproject.toml")
    return tomllib.loads(path.read_text())["project"]["version"]


def uv_lock() -> str:
    path = pathlib.Path("uv.lock")
    packages = tomllib.loads(path.read_text())["package"]
    return next(package["version"] for package in packages if package["name"] == "frekvens")


def version_h() -> str:
    path = pathlib.Path("firmware") / "include" / "config" / "version.h"
    match = re.search(r'^#define\sVERSION\s"([^"]+)"$', path.read_text(), re.MULTILINE)
    return match.group(1) if match else ""


def main() -> None:
    versions = {
        "firmware/include/config/version.h": version_h(),
        "scripts/src/config/version.py": VERSION,
        "webapp/package.json": package_json(),
        "webapp/package-lock.json": package_lock_json(),
        "pyproject.toml": pyproject_toml(),
        "uv.lock": uv_lock(),
    }
    normalized = {}
    for name, value in versions.items():
        print(f"{value} @ {name}")
        normalized[name] = packaging.version.Version(value)
    first = next(iter(normalized.values()))
    for version in normalized.values():
        if version != first:
            raise ValueError("Version mismatch")


if __name__ == "__main__":
    main()
