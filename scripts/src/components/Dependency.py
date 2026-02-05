import httpx
import logging
import packaging.version
import typing

from ..config.version import VERSION

if typing.TYPE_CHECKING:
    from ..Frekvens import Frekvens


class Dependency:
    project: "Frekvens"

    def __init__(self, project: "Frekvens") -> None:
        self.project = project

    def initialize(self) -> None:
        local = packaging.version.parse(VERSION)
        try:
            latest = packaging.version.parse(
                httpx.get(
                    "https://api.github.com/repos/VIPnytt/Frekvens/releases/latest",
                    headers={
                        "Accept": "application/vnd.github+json",
                        "User-Agent": f"Frekvens/{VERSION} (+https://github.com/VIPnytt/Frekvens)",
                        "X-GitHub-Api-Version": "2022-11-28",
                    },
                )
                .raise_for_status()
                .json()["tag_name"]
            )
            if latest > local:
                print(
                    f"\033[93m[notice] A new release of Frekvens is available: {local.public} -> {latest.public}\033[0m"
                )
                print("Release notes: https://github.com/VIPnytt/Frekvens/releases/latest")
        except httpx.HTTPError as e:
            logging.warning("Update check failed: %s", e)
