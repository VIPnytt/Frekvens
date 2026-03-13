import os
import packaging.version
import pathlib
import re
import subprocess


def main() -> None:
    allowed = tuple(
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
    neutral = tuple(
        re.compile(pattern)
        for pattern in (
            r"^git\+https://github\.com/[^/]+/[^/]+\.git#([^/]+)$",  # GitHub git https tag
            r"^git\+ssh://github\.com/[^/]+/[^/]+\.git#([^/]+)$",  # GitHub git SSH tag
            r"^https://api\.github\.com/repos/[^/]+/[^/]+/(?:tar|zip)ball/([^/]+)$",  # GitHub API tag
            r"^https://github\.com/[^/]+/[^/]+/archive/refs/tags/([^/]+)\.(?:tar\.gz|zip)$",  # GitHub archive tag
        )
    )
    banned = tuple(
        re.compile(pattern)
        for pattern in (
            r"^git\+https://github\.com/[^/]+/[^/]+\.git$",  # GitHub git https
            r"^git\+ssh://github\.com/[^/]+/[^/]+\.git$",  # GitHub git SSH
            r"^https://github\.com/[^/]+/[^/]+/archive/refs/heads/[^/]+\.(?:tar\.gz|zip)$",  # GitHub archive head
        )
    )
    blacklist = {
        "dev",
        "develop",
        "main",
        "master",
    }
    fail = False
    seen: set[tuple[str, str]] = set()
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
        key = (package, requirement)
        if key in seen:
            continue
        seen.add(key)
        print(line)
        if (
            requirement.endswith(f"/{package}")
            or any(pattern.match(requirement) for pattern in allowed)
            or pathlib.Path(requirement).exists()
        ):
            continue
        match = None
        for pattern in neutral:
            match = pattern.match(requirement)
            if match:
                break
        ref = match.group(1) if match else None
        if ref is not None:
            try:
                packaging.version.Version(ref)
                continue
            except packaging.version.InvalidVersion:
                print(f"::debug::Invalid version: {ref}")
        if (
            (ref is not None and ref.lower() in blacklist)
            or any(pattern.match(requirement) for pattern in banned)
            or any(char in requirement for char in "!,<=>^~")
        ):
            print(f"::error::Unpinned dependency: {requirement}")
            fail = True
            continue
        print(f"::warning::Unclassified format: {requirement}")
    if fail:
        raise SystemExit(1)


if __name__ == "__main__":
    main()
