import datetime
import httpx
import json
import logging
import os
import packaging.version
import pathlib
import re
import typing


class Context:
    client: httpx.Client
    logger: logging.Logger
    major: int
    minor: int
    now: datetime.datetime
    patch: int
    workspace: pathlib.Path

    def __init__(self) -> None:
        self.client = httpx.Client(
            base_url="https://api.registry.platformio.org",
            follow_redirects=True,
            headers={
                "Accept": "application/json",
                "User-Agent": "Frekvens (+https://github.com/VIPnytt/Frekvens)",
            },
            max_redirects=1,
        )
        self.logger = logging.getLogger("Frekvens")
        self.major = int(os.environ.get("semver-major-days", 0))
        self.minor = int(os.environ.get("semver-minor-days", 0))
        self.now = datetime.datetime.now(datetime.timezone.utc)
        self.patch = int(os.environ.get("semver-patch-days", 0))
        self.workspace = pathlib.Path(os.environ.get("GITHUB_WORKSPACE", os.getcwd()))

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.client.close()


class Client:
    ctx: Context
    regex: re.Pattern[str]

    def __init__(self, ctx: Context) -> None:
        self.ctx = ctx


class HandlerFragment(typing.TypedDict):
    name: str
    owner: str
    type: str
    version_new: str


class BaseFragment(HandlerFragment):
    string_old: str
    version_old: str


class ResultFragment(BaseFragment):
    string_new: str


class ChecksumData(typing.TypedDict):
    sha256: str


class FileData(typing.TypedDict):
    download_url: str
    checksum: ChecksumData
    name: str


class ReleaseData(typing.TypedDict):
    files: list[FileData]
    name: str
    released_at: str
    type: str


class Handler:
    ctx: Context
    ignore: set[str]
    name: str
    owner: str
    releases: list[ReleaseData]
    tag: str
    type: str
    version: packaging.version.Version

    def __init__(self, ctx: Context, type: str, owner: str, name: str) -> None:
        self.ctx = ctx
        data = self.ctx.client.get(f"/v3/packages/{owner}/{type}/{name}").raise_for_status().json()
        self.name = data["name"]
        self.owner = data["owner"]["username"]
        self.releases = data["versions"]
        self.type = data["type"]

    def parse_release(self, release: ReleaseData) -> packaging.version.Version | None:
        if release["name"] in self.ignore:
            return None
        try:
            return packaging.version.Version(release["name"])
        except packaging.version.InvalidVersion:
            self.ctx.logger.debug("Invalid version: %s/%s @ %s", self.owner, self.name, release["name"], exc_info=True)
            self.ignore.add(release["name"])
            return None

    def is_candidate(self, release: ReleaseData) -> bool:
        version = self.parse_release(release)
        if not version or version <= self.version:
            return False
        elif version.is_prerelease and not self.version.is_prerelease:
            self.ctx.logger.info("Skipping: %s/%s @ %s", self.owner, self.name, release["name"])
            return False
        elif (
            (
                version.major == self.version.major
                and version.minor == self.version.minor
                and version.micro == self.version.micro
            )
            or self.is_mature_patch(version)
            or self.is_mature_minor(version)
            or self.is_mature_major(version)
        ):
            return True
        self.ctx.logger.info("On cooldown: %s/%s @ %s", self.owner, self.name, release["name"])
        return False

    def get_age(self, release: ReleaseData) -> int:
        try:
            return (self.ctx.now - datetime.datetime.fromisoformat(release["released_at"])).days
        except ValueError:
            self.ctx.logger.debug(
                "Invalid timestamp: %s/%s @ %s", self.owner, self.name, release["name"], exc_info=True
            )
            self.ignore.add(release["name"])
            return 0

    def is_mature_major(self, version: packaging.version.Version) -> bool:
        if version.major > self.version.major:
            for release in self.releases:
                v_release = self.parse_release(release)
                if (
                    v_release
                    and v_release.major == version.major
                    and (not v_release.is_prerelease or self.version.is_prerelease)
                    and self.get_age(release) >= self.ctx.major
                ):
                    return True
        return False

    def is_mature_minor(self, version: packaging.version.Version) -> bool:
        if version.major == self.version.major and version.minor > self.version.minor:
            for release in self.releases:
                v_release = self.parse_release(release)
                if (
                    v_release
                    and v_release.major == version.major
                    and v_release.minor == version.minor
                    and (not v_release.is_prerelease or self.version.is_prerelease)
                    and self.get_age(release) >= self.ctx.minor
                ):
                    return True
        return False

    def is_mature_patch(self, version: packaging.version.Version) -> bool:
        if (
            version.major == self.version.major
            and version.minor == self.version.minor
            and version.micro > self.version.micro
        ):
            for release in self.releases:
                v_release = self.parse_release(release)
                if (
                    v_release
                    and v_release.major == version.major
                    and v_release.minor == version.minor
                    and v_release.micro == version.micro
                    and (not v_release.is_prerelease or self.version.is_prerelease)
                    and self.get_age(release) >= self.ctx.patch
                ):
                    return True
        return False


def main() -> None:
    import File  # noqa: E402
    import Package  # noqa: E402

    logging.basicConfig()
    logging.getLogger("Frekvens").setLevel(logging.INFO)
    with Context() as ctx:
        results = []
        results.extend(File.Download(ctx).matrix())
        results.extend(Package.Registry(ctx).matrix())
        print(json.dumps(results))


if __name__ == "__main__":
    main()
