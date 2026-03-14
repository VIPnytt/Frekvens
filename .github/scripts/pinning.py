import json
import os
import packaging.version
import pathlib
import re
import subprocess
import typing


class PlatformPackage(typing.TypedDict):
    version: str


class PlatformManifest(typing.TypedDict):
    packages: dict[str, PlatformPackage]


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
    platform_manifests: list[PlatformManifest]

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
        self.platform_manifests = self._load_platforms()

    def _load_platforms(self) -> list[PlatformManifest]:
        platforms: list[PlatformManifest] = []
        for platform in json.loads(
            subprocess.run(
                ["pio", "platform", "list", "--json-output"],
                capture_output=True,
                check=True,
                text=True,
            ).stdout
        ):
            path = pathlib.Path(platform["__pkg_dir"]) / "platform.json"
            platforms.append(json.loads(path.read_text()))
        return platforms

    def parse(self) -> list[tuple[str, str]]:
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
            parsed = self._parse_package_line(line)
            if parsed is None:
                continue
            package, requirement = parsed
            key = (package, requirement)
            if key in seen_packages:
                continue
            seen_packages.add(key)
            resolved = self._resolve_requirements(requirement)
            if resolved:
                for version in resolved:
                    self._append_package(packages, package, requirement, line, version)
            else:
                self._append_package(packages, package, requirement, line)
        return packages
    
    def _parse_package_line(self, line: str) -> tuple[str, str] | None:
        if not line.endswith(")"):
            return None
        if " (required: " not in line:
            return None
        raw_package, _, raw_requirement = line.partition(" (required: ")
        package = raw_package.removeprefix("Platform ").lstrip(" ─│└├")
        requirement = raw_requirement.removesuffix(")")
        return package, requirement
    
    def _resolve_requirements(self, requirement: str) -> list[str]:
        path = pathlib.Path(requirement) / "package.json"
        if not path.is_file():
            return []
        name = json.loads(path.read_text())["name"]
        versions: list[str] = []
        for manifest in self.platform_manifests:
            if name in manifest["packages"]:
                versions.append(manifest["packages"][name]["version"])
        return versions

    def _append_package(
        self, packages: list[tuple[str, str]], package: str, requirement: str, line: str, resolved: str | None = None
    ) -> None:
        if resolved is None:
            packages.append((package, requirement))
            print(line)
            return
        packages.append((package, resolved))
        print(line.replace(requirement, resolved, 1))

    def check(self) -> bool:
        pinned = True
        reported: set[str] = set()
        for package, requirement in self.parse():
            if requirement.endswith(f"/{package}") or any(pattern.match(requirement) for pattern in self.allowed):
                continue
            ref = self._neutral_ref(requirement)
            if ref is not None:
                try:
                    packaging.version.Version(ref)
                    continue
                except packaging.version.InvalidVersion:
                    self._report(reported, ref, f"::debug::Invalid version: {ref}")
            if self._is_unpinned(requirement, ref):
                pinned = False
                self._report(reported, requirement, f"::error::Unpinned dependency: {requirement}")
                continue
            self._report(reported, requirement, f"::warning::Unclassified format: {requirement}")
        return pinned

    def _neutral_ref(self, requirement: str) -> str | None:
        for pattern in self.neutral:
            match = pattern.match(requirement)
            if match:
                return match.group(1)
        return None

    def _is_unpinned(self, requirement: str, ref: str | None) -> bool:
        return (
            (ref is not None and ref.lower() in self.blacklist)
            or any(pattern.match(requirement) for pattern in self.banned)
            or any(char in requirement for char in "!,<=>^~")
        )

    def _report(self, reported: set[str], key: str, message: str) -> None:
        if key not in reported:
            reported.add(key)
            print(message)


def main() -> None:
    if not Pinning().check():
        raise SystemExit(1)


if __name__ == "__main__":
    main()
