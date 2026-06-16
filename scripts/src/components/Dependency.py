import json
import logging
import packaging.version
import typing
import urllib.error
import urllib.request

from ..config.version import VERSION

if typing.TYPE_CHECKING:
    from ..Frekvens import Frekvens


class Dependency:
    project: "Frekvens"

    def __init__(self, project: "Frekvens") -> None:
        self.project = project

    def initialize(self) -> None:
        try:
            tag = json.load(
                urllib.request.urlopen(
                    urllib.request.Request(
                        "https://api.github.com/repos/VIPnytt/Frekvens/releases/latest",
                        headers={
                            "Accept": "application/vnd.github+json",
                            "User-Agent": f"Frekvens/{VERSION} (+https://github.com/VIPnytt/Frekvens)",
                            "X-GitHub-Api-Version": "2022-11-28",
                        },
                    )
                )
            )["tag_name"]
            local = packaging.version.parse(VERSION)
            latest = packaging.version.parse(tag)
            if latest > local:
                print(
                    f"\033[93m[notice] A new release of Frekvens is available: {local.public} -> {latest.public}\033[0m"
                )
                print(f"Release notes: https://github.com/VIPnytt/Frekvens/releases/tag/{tag}")
        except urllib.error.URLError as e:
            logging.debug("Update check failed: %s", e)
