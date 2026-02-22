import httpx
import packaging.version
import PlatformIo
import re


class HandlerFragment(PlatformIo.HandlerFragment):
    checksum_new: str
    file_new: str


class BaseFragment(HandlerFragment, PlatformIo.BaseFragment):
    pass


class ResultFragment(BaseFragment, PlatformIo.ResultFragment):
    pass


class Download(PlatformIo.Client):
    def __init__(self, ctx: PlatformIo.Context) -> None:
        super().__init__(ctx)
        self.regex = re.compile(
            r"https://dl\.registry\.platformio\.org/download/(?P<owner>[^/;\s]+)/(?P<type>[^/;\s]+)/(?P<name>[^/;\s]+)/(?P<version>[^/;\s]+)/(?P<file>[^/;\s]+)(?:\s*;\s*sha256:[0-9a-f]{64})?$",
            re.MULTILINE,
        )

    def matrix(self) -> list[ResultFragment]:
        seen: set[str] = set()
        fragments: list[BaseFragment] = []
        with open(self.ctx.workspace / "platformio.ini", encoding="utf-8") as f:
            for match in self.regex.finditer(f.read()):
                string = match.group(0)
                if string in seen:
                    continue
                seen.add(string)
                try:
                    pending = Handler(self.ctx, match["type"], match["owner"], match["name"]).check(
                        match["version"], match["file"]
                    )
                except (httpx.HTTPError, packaging.version.InvalidVersion):
                    self.ctx.logger.error(
                        "Update check failed: %s/%s @ %s",
                        match["owner"],
                        match["name"],
                        match["version"],
                        exc_info=True,
                    )
                    continue
                if pending:
                    fragments.append(
                        {
                            "owner": pending["owner"],
                            "name": pending["name"],
                            "version_old": match["version"],
                            "version_new": pending["version_new"],
                            "type": pending["type"],
                            "file_new": pending["file_new"],
                            "checksum_new": pending["checksum_new"],
                            "string_old": string,
                        }
                    )
        return [
            ResultFragment(
                {
                    "owner": fragment["owner"],
                    "name": fragment["name"],
                    "version_old": fragment["version_old"],
                    "version_new": fragment["version_new"],
                    "file_new": fragment["file_new"],
                    "type": fragment["type"],
                    "checksum_new": fragment["checksum_new"],
                    "string_old": fragment["string_old"],
                    "string_new": f"{fragment['file_new']} ; sha256:{fragment['checksum_new']}",
                }
            )
            for fragment in fragments
        ]


class Handler(PlatformIo.Handler):
    def check(self, tag: str, file: str) -> HandlerFragment | None:
        self.file = file
        self.ignore = set()
        self.tag = tag
        self.version = packaging.version.Version(self.tag)
        for release in self.releases:
            if self.is_candidate(release):
                self.ctx.logger.info("Update available: %s/%s @ %s", self.owner, self.name, release["name"])
                _file = self.parse_files(release)
                if _file:
                    return HandlerFragment(
                        {
                            "checksum_new": _file["checksum"]["sha256"],
                            "file_new": _file["download_url"],
                            "name": self.name,
                            "owner": self.owner,
                            "type": self.type,
                            "version_new": release["name"],
                        }
                    )
                self.ctx.logger.warning("Asset not found: %s/%s @ %s", self.owner, self.name, release["name"])
                return None
        self.ctx.logger.info("Up to date: %s/%s @ %s", self.owner, self.name, self.tag)
        return None

    def parse_files(self, release: PlatformIo.ReleaseData) -> PlatformIo.FileData | None:
        for file in release["files"]:
            if file["name"] == self.file or file["name"] == self.file.replace(self.tag, release["name"], 1):
                return file
        return None
