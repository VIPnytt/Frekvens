import httpx
import logging
import os
import packaging.version
import re
import typing
import urllib.parse

from ..config.version import VERSION

if typing.TYPE_CHECKING:
    from ..Frekvens import Frekvens


class Dependency:
    github: httpx.Client
    project: "Frekvens"
    proceed: bool = True

    def __init__(self, project: "Frekvens") -> None:
        self.project = project
        self.github = httpx.Client(
            base_url="https://api.github.com",
            headers={
                "Accept": "application/vnd.github+json",
                "User-Agent": f"Frekvens/{VERSION} (+https://github.com/VIPnytt/Frekvens)",
                "X-GitHub-Api-Version": "2022-11-28",
            },
        )

    def initialize(self) -> None:
        local = packaging.version.parse(VERSION)
        try:
            latest = packaging.version.parse(
                self.github.get("/repos/VIPnytt/Frekvens/releases/latest")
                .raise_for_status()
                .json()["tag_name"]
            )
            if latest > local:
                print(
                    f"\033[93m[notice] A new release of Frekvens is available: {local.public} -> {latest.public}\033[0m"
                )
                print(
                    "Release notes: https://github.com/VIPnytt/Frekvens/releases/latest"
                )
                self.proceed = False
        except httpx.HTTPError as e:
            logging.warning("Update check failed: %s", e)
            self.proceed = False

    def validate(self) -> None:
        if self.proceed and packaging.version.parse(VERSION).is_devrelease:
            self._check("platform", self.project.env.GetProjectOption("platform"))
            if self.proceed:
                for pkg in self.project.env.GetProjectOption("platform_packages", []):
                    _, _, uri = pkg.partition("@")
                    self._check("tool", uri.strip())
                    if not self.proceed:
                        return
                for dep in self.project.env.GetProjectOption("lib_deps"):
                    self._check("library", dep)
                    if not self.proceed:
                        return

    def _check(self, type: str, query: str) -> bool | None:
        parsed = urllib.parse.urlparse(query, allow_fragments=False)
        try:
            return (
                self._github(query)
                if parsed.scheme and parsed.netloc and parsed.hostname == "github.com"
                else self._platformio(type, query)
            )
        except httpx.HTTPError as e:
            logging.warning("Dependency update check failed: %s", e)
            self.proceed = False
        except packaging.version.InvalidVersion as e:
            logging.warning("Dependency update check failed: %s", e)
        return None

    def _github(self, url: str) -> bool | None:
        match = re.search(
            r"^(?:git\+)?https://github\.com/(?P<repository>[^/]+/[^/#]+)(?:\.git\#(?:(?P<sha_git>[0-9a-fA-F]{7,40})|(?P<tag_git>[A-Za-z0-9._-]+))|/(?:archive/(?:(?P<sha_archive>[0-9a-fA-F]{7,40})|refs/tags/(?P<tag_archive>[A-Za-z0-9._-]+))\.(?:zip|tar\.gz)|releases/download/(?P<tag_release>[A-Za-z0-9._-]+)/[^/]+\.(?:zip|tar\.gz)))$",
            url,
        )
        if not match:
            logging.debug("Unsupported GitHub dependency format: %s", url)
            return None
        repository = match.group("repository")
        local_sha = match.group("sha_archive") or match.group("sha_git")
        local_tag = (
            match.group("tag_archive")
            or match.group("tag_git")
            or match.group("tag_release")
        )
        if not local_tag:
            with open(os.path.join("platformio.ini"), encoding="utf-8") as ini:
                for line in ini:
                    text, _, comment = line.partition(";")
                    if url in text and comment:
                        local_tag = comment.split(maxsplit=1)[0]
                        break
        latest_tag: str = (
            self.github.get(f"/repos/{repository}/releases/latest")
            .raise_for_status()
            .json()["tag_name"]
        )
        latest_version = packaging.version.Version(latest_tag)
        if local_tag:
            local_version = packaging.version.Version(local_tag)
            if latest_version > local_version:
                print(
                    f"Dependency update available: {repository}, {local_version.public} → {latest_version.public}"
                )
                return True
            return False
        if local_sha:
            latest_sha: str = (
                self.github.get(f"/repos/{repository}/commits/{latest_tag}")
                .raise_for_status()
                .json()["sha"]
            )
            status = (
                self.github.get(
                    f"/repos/{repository}/compare/{local_sha}...{latest_sha}"
                )
                .raise_for_status()
                .json()["status"]
            )
            if status == "behind":
                print(
                    f"Dependency update available: {repository}, {latest_version.public}"
                )
                return True
            return False
        return None

    def _platformio(self, type: str, query: str) -> bool | None:
        match = re.search(
            r"^(?P<owner>[\w]+)/(?P<package>[\w]+)\s*@\s*[!<=>^~]*\s*(?P<version>[0-9]+(?:\.[0-9]+)*)(,.*)?$",
            query,
        )
        if not match:
            logging.debug("Unsupported PlatformIO dependency format: %s", query)
            return None
        owner = match.group("owner")
        package = match.group("package")
        local = packaging.version.Version(match.group("version"))
        latest = packaging.version.Version(
            httpx.get(
                f"https://api.registry.platformio.org/v3/packages/{owner}/{type}/{package}",
                headers={
                    "Accept": "application/json",
                    "User-Agent": self.github.headers["User-Agent"],
                },
            )
            .raise_for_status()
            .json()["version"]["name"]
        )
        if latest > local:
            print(
                f"Dependency update available: {owner}/{package}, {local.public} → {latest.public}"
            )
            return True
        return False
