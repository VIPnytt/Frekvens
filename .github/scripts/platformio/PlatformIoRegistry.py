import datetime
import httpx
import json
import logging
import packaging.version
import re
import subprocess
import typing


class PlatformIoRegistry:
    fragments: list[typing.Tuple[str, str, str, str, str, str]] = []
    logger: logging.Logger
    major: int = 90
    minor: int = 21
    patch: int = 3
    rest: httpx.Client
    seen: set[str] = set()

    def __init__(self) -> None:
        self.logger = logging.getLogger("Frekvens")
        self.rest = httpx.Client(
            base_url="https://api.registry.platformio.org",
            headers={
                "Accept": "application/json",
                "User-Agent": "Frekvens (+https://github.com/VIPnytt/Frekvens)",
            },
        )
        self.regex = re.compile(r"^(?P<owner>[a-z0-9_-]+)\/(?P<name>[A-Za-z0-9_-]+)\s*@\s*(?P<version>\d+\.\d+\.\d+)$")

    def matrix(self) -> str:
        for _, entries in json.loads(
            subprocess.run(
                ["pio", "project", "config", "--json-output"], capture_output=True, check=True, text=True
            ).stdout
        ):
            section = dict(entries)
            if "platform" in section:
                self.parse("platform", section["platform"])
            if "platform_packages" in section:
                for package in section["platform_packages"]:
                    self.parse("tool", package)
            if "lib_deps" in section:
                for lib in section["lib_deps"]:
                    self.parse("library", lib)
        return json.dumps(
            [
                {
                    "type": type,
                    "owner": owner,
                    "name": name,
                    "version_old": version_old,
                    "string_old": string_old,
                    "version_new": version_new,
                    "string_new": f"{owner}/{name} @ {version_new}",
                }
                for type, owner, name, version_old, version_new, string_old in self.fragments
            ]
        )

    def parse(self, type: str, query: str) -> None:
        if query in self.seen:
            return
        self.seen.add(query)
        match = self.regex.fullmatch(query)
        if match:
            pending = self.check(type, match["owner"], match["name"], match["version"])
            if pending:
                self.fragments.append(
                    (
                        type,
                        match["owner"],
                        match["name"],
                        match["version"],
                        pending,
                        match.group(0),
                    )
                )
        else:
            self.logger.debug("Unsupported format: %s", query)

    def check(self, type: str, owner: str, name: str, version: str) -> typing.Optional[str]:
        try:
            v_version = packaging.version.Version(version)
            package = self.rest.get(f"/v3/packages/{owner}/{type}/{name}").raise_for_status().json()
            for release in package["versions"]:
                try:
                    v_package_version = packaging.version.Version(release["name"])
                    if v_package_version <= v_version:
                        continue
                    elif v_package_version.is_prerelease:
                        self.logger.info("Skipping pre-release: %s/%s %s", owner, name, release["name"])
                        continue
                    days = (
                        datetime.datetime.now(datetime.timezone.utc)
                        - datetime.datetime.fromisoformat(release["released_at"].replace("Z", "+00:00"))
                    ).days
                    if (
                        (days < self.major and v_package_version.major > v_version.major)
                        or (
                            days < self.minor
                            and v_package_version.minor > v_version.minor
                            and v_package_version.major == v_version.major
                        )
                        or (
                            days < self.patch
                            and v_package_version.minor == v_version.minor
                            and v_package_version.major == v_version.major
                        )
                    ):
                        self.logger.info("On cooldown, skipping: %s/%s %s", owner, name, release["name"])
                        continue
                    self.logger.info("Update available: %s/%s %s", owner, name, release["name"])
                    return release["name"]
                except packaging.version.InvalidVersion as e:
                    self.logger.warning(e)
        except (httpx.HTTPError, packaging.version.InvalidVersion) as e:
            self.logger.error(e)
        self.logger.info("Up to date: %s/%s %s", owner, name, version)
        return None


def main() -> None:
    logging.basicConfig()
    logging.getLogger("Frekvens").setLevel(logging.INFO)
    print(PlatformIoRegistry().matrix())


if __name__ == "__main__":
    main()
