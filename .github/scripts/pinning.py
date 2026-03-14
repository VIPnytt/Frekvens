import json
import os
import packaging.version
import pathlib
import re
import subprocess


class Pinning:
    allowed: tuple[re.Pattern, ...]
    banned: tuple[re.Pattern, ...]
    blacklist: set[str] = {
        "dev",
        "develop",
        "main",
        "master",
    }
    neutral: tuple[re.Pattern, ...]
    requirements: dict[str, list[str]]

    def __init__(self) -> None:
        self.allowed = tuple(
            re.compile(pattern)
            for pattern in (
                r"^git\+https://github\.com/[^/]+/[^/]+\.git#[0-9a-f]{40}$",  # GitHub git https commit
                r"^git\+ssh://github\.com/[^/]+/[^/]+\.git#[0-9a-f]{40}$",  # GitHub git SSH commit
                r"^https://api\.github\.com/repos/[^/]+/[^/]+/(?:tar|zip)ball/[0-9a-f]{40}$",  # GitHub API commit
                r"^https://dl\.registry\.platformio\.org/download/[^/]+/[^/]+/[^/]+/[^/]+/[^/]+$",  # PlatformIO registry download
                r"^https://github\.com/[^/]+/[^/]+/archive/[0-9a-f]{40}\.(?:tar\.gz|zip)$",  # GitHub archive commit
                r"^https://github\.com/[^/]+/[^/]+/releases/download/[^/]+/[^/]+$",  # GitHub release asset
            )
        )
        self.neutral = tuple(
            re.compile(pattern)
            for pattern in (
                r"^git\+https://github\.com/[^/]+/[^/]+\.git#([^/]+)$",  # GitHub git https tag
                r"^git\+ssh://github\.com/[^/]+/[^/]+\.git#([^/]+)$",  # GitHub git SSH tag
                r"^https://api\.github\.com/repos/[^/]+/[^/]+/(?:tar|zip)ball/([^/]+)$",  # GitHub API tag
                r"^https://github\.com/[^/]+/[^/]+/archive/refs/tags/([^/]+)\.(?:tar\.gz|zip)$",  # GitHub archive tag
            )
        )
        self.banned = tuple(
            re.compile(pattern)
            for pattern in (
                r"^git\+https://github\.com/[^/]+/[^/]+\.git$",  # GitHub git https
                r"^git\+ssh://github\.com/[^/]+/[^/]+\.git$",  # GitHub git SSH
                r"^https://github\.com/[^/]+/[^/]+/archive/refs/heads/[^/]+\.(?:tar\.gz|zip)$",  # GitHub archive head
            )
        )
        platforms = json.loads(
            subprocess.run(
                ["pio", "platform", "list", "--json-output"],
                capture_output=True,
                check=True,
                text=True,
            ).stdout
        )
        self.requirements = {}
        for manifest in [
            json.loads((pathlib.Path(platform["__pkg_dir"]) / "platform.json").read_text()) for platform in platforms
        ]:
            for name, package in manifest["packages"].items():
                self.requirements.setdefault(name, []).append(package["version"])

    def collect(self) -> list[tuple[str, str]]:
        packages: list[tuple[str, str]] = []
        seen_packages: set[tuple[str, str]] = set()
        for line in subprocess.run(
            ["pio", "pkg", "list"],
            env={**os.environ, "PYTHONIOENCODING": "utf-8"} if os.name == "nt" else None,
            capture_output=True,
            check=True,
            encoding="utf-8",
            text=True,
        ).stdout.splitlines():
            if not line.endswith(")") or " (required: " not in line:
                continue
            _package, _, _requirement = line.partition(" (required: ")
            package = _package.removeprefix("Platform ").lstrip(" ─│└├")
            requirement = _requirement.removesuffix(")")
            entry = (package, requirement)
            if entry in seen_packages:
                continue
            seen_packages.add(entry)
            path = pathlib.Path(requirement) / "package.json"
            if path.is_file():
                name = json.loads(path.read_text())["name"]
                resolved = self.requirements.get(name, [])
            else:
                resolved = []
            if resolved:
                for version in resolved:
                    packages.append((package, version))
                    print(line.replace(requirement, version, 1))
            else:
                packages.append(entry)
                print(line)
        return packages

    def check(self) -> bool:
        pinned = True
        reported: set[str] = set()
        for package, requirement in self.collect():
            if requirement.endswith(f"/{package}") or any(pattern.match(requirement) for pattern in self.allowed):
                continue
            ref = next(
                (match.group(1) for pattern in self.neutral if (match := pattern.match(requirement))),
                None,
            )
            if ref is not None:
                try:
                    packaging.version.Version(ref)
                    continue
                except packaging.version.InvalidVersion:
                    if ref not in reported:
                        reported.add(ref)
                        print(f"::debug::Invalid version: {ref}")
            if (
                (ref is not None and ref.lower() in self.blacklist)
                or any(pattern.match(requirement) for pattern in self.banned)
                or any(char in requirement for char in "!,<=>^~")
            ):
                pinned = False
                if requirement not in reported:
                    reported.add(requirement)
                    print(f"::error::Unpinned dependency: {requirement}")
                continue
            if requirement not in reported:
                reported.add(requirement)
                print(f"::warning::Unclassified format: {requirement}")
        return pinned


def main() -> int:
    return 0 if Pinning().check() else 1


if __name__ == "__main__":
    raise SystemExit(main())
