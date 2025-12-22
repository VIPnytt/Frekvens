import httpx
import logging
import os
import packaging.version
import re
import typing

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
                self.proceed = False
        except httpx.HTTPError as e:
            logging.warning("Update check failed: %s", e)
            self.proceed = False

    def validate(self) -> None:
        if self.proceed and packaging.version.parse(VERSION).is_devrelease:
            try:
                self._check(self.project.env.GetProjectOption("platform"))
                for dependency in self.project.env.GetProjectOption("lib_deps"):
                    self._check(dependency)
            except httpx.HTTPError as e:
                logging.warning("Dependency update check failed: %s", e)
                self.proceed = False

    def _check(self, dependency: str) -> None:
        match = re.compile(
            r"https://github\.com/"
            r"(?P<repository>[^/]+/[^/]+)/"
            r"archive/(?P<ref>.+?)\.(?:tar\.gz|zip)$"
        ).search(dependency)
        if not match:
            return
        repository = match.group("repository")
        ref = match.group("ref")
        local_tag = (
            ref.split("refs/tags/", 1)[1]
            if ref.startswith("refs/tags/")
            else (None if re.fullmatch(r"[0-9a-fA-F]{7,40}", ref) else ref)
        )
        if not local_tag:
            with open(
                os.path.join(self.project.env["PROJECT_DIR"], "platformio.ini"),
                encoding="utf-8",
            ) as ini:
                for line in ini:
                    if dependency in line and ";" in line:
                        comment = line.split(";", 1)[1].strip()
                        if comment:
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
                    f"{repository}: update available, {local_version.public} → {latest_version.public}"
                )
            return
        local_sha = None if re.fullmatch(r"[0-9a-fA-F]{7,40}", ref) else ref
        if local_sha:
            if (
                self.github.get(
                    f"/repos/{repository}/compare/{local_sha}...{self.github.get(f'/repos/{repository}/commits/{latest_tag}').raise_for_status().json()['sha']}"
                )
                .raise_for_status()
                .json()["status"]
                == "behind"
            ):
                print(f"{repository}: update available, {latest_version.public}")
