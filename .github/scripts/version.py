import json
import packaging.version
import pathlib
import re
import tomllib


class Consistency:
    pep: list[tuple[pathlib.Path, str]]
    semver: list[tuple[pathlib.Path, str]]

    def __init__(self) -> None:
        self.pep = [
            self._pyproject_toml(),
            self._uv_lock(),
        ]
        self.semver = [
            self._package_json(),
            self._package_lock_json(),
            self._version_h(),
            self._version_py(),
        ]

    def check(self) -> bool:
        cwd = pathlib.Path()
        for path, version in sorted(
            self.semver + self.pep,
            key=lambda pair: (pair[0].parent == cwd, pair[0].parent.absolute(), pair[0].name),
        ):
            print(f"{version} @ {path}")
        semver = self.semver[0][1]
        pep = str(packaging.version.Version(semver))
        return all(version == semver for _, version in self.semver) and all(version == pep for _, version in self.pep)

    def _package_json(self) -> tuple[pathlib.Path, str]:
        path = pathlib.Path("webapp") / "package.json"
        return path, json.loads(path.read_text())["version"]

    def _package_lock_json(self) -> tuple[pathlib.Path, str]:
        path = pathlib.Path("webapp") / "package-lock.json"
        return path, json.loads(path.read_text())["packages"][""]["version"]

    def _pyproject_toml(self) -> tuple[pathlib.Path, str]:
        path = pathlib.Path("pyproject.toml")
        return path, tomllib.loads(path.read_text())["project"]["version"]

    def _uv_lock(self) -> tuple[pathlib.Path, str]:
        path = pathlib.Path("uv.lock")
        packages = tomllib.loads(path.read_text())["package"]
        return path, next(package["version"] for package in packages if package["name"] == "frekvens")

    def _version_h(self) -> tuple[pathlib.Path, str]:
        path = pathlib.Path("firmware") / "include" / "config" / "version.h"
        match = re.search(r'^#define\sVERSION\s"([^"]+)"$', path.read_text(), re.MULTILINE)
        return path, match.group(1) if match else ""

    def _version_py(self) -> tuple[pathlib.Path, str]:
        path = pathlib.Path("scripts") / "src" / "config" / "version.py"
        match = re.search(r'^VERSION:\sstr\s=\s"([^"]+)"$', path.read_text(), re.MULTILINE)
        return path, match.group(1) if match else ""


def main() -> int:
    return 0 if Consistency().check() else 1


if __name__ == "__main__":
    raise SystemExit(main())
