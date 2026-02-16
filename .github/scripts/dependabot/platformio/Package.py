import httpx
import json
import packaging.version
import PlatformIo
import re
import subprocess


class PackageRegistry:
    ctx: PlatformIo.Context
    fragments: list[PlatformIo.BaseFragment]
    regex: re.Pattern[str]
    seen: dict[str, set[str]]

    def __init__(self, ctx: PlatformIo.Context) -> None:
        self.ctx = ctx
        self.regex = re.compile(
            r"^(?P<owner>[^/]+)/(?P<name>[^/]+?)\s*@\s*(?:[\^~>=<!]{,2})\s*(?P<version>\d+\.\d+\.\d+\S*?)(?:,.*)?$"
        )

    def matrix(self) -> list[PlatformIo.ResultFragment]:
        self.fragments = []
        self.seen = {}
        for _, tables in json.loads(
            subprocess.run(
                ["pio", "project", "config", "--json-output"],
                capture_output=True,
                check=True,
                cwd=self.ctx.workspace,
                text=True,
            ).stdout
        ):
            table = dict(tables)
            if "platform" in table:
                self.handle("platform", table["platform"])
            if "platform_packages" in table:
                for platform_package in table["platform_packages"]:
                    self.handle("tool", platform_package)
            if "lib_deps" in table:
                for lib_dep in table["lib_deps"]:
                    self.handle("library", lib_dep)
        return [
            PlatformIo.ResultFragment(
                {
                    "owner": fragment["owner"],
                    "name": fragment["name"],
                    "version_old": fragment["version_old"],
                    "version_new": fragment["version_new"],
                    "type": fragment["type"],
                    "string_old": fragment["string_old"],
                    "string_new": f"{fragment['owner']}/{fragment['name']} @ {fragment['version_new']}",
                }
            )
            for fragment in self.fragments
        ]

    def handle(self, type: str, string: str) -> None:
        if string in self.seen.setdefault(type, set()):
            return
        self.seen[type].add(string)
        match = self.regex.fullmatch(string)
        if match:
            try:
                pending = Handler(self.ctx, type, match["owner"], match["name"]).check(match["version"])
            except (httpx.HTTPError, packaging.version.InvalidVersion):
                self.ctx.logger.error(
                    "Update check failed: %s/%s @ %s", match["owner"], match["name"], match["version"], exc_info=True
                )
                return
            if pending:
                self.fragments.append(
                    PlatformIo.BaseFragment(
                        {
                            "owner": match["owner"],
                            "name": match["name"],
                            "version_old": match["version"],
                            "version_new": pending["version_new"],
                            "type": type,
                            "string_old": string,
                        }
                    )
                )


class Handler(PlatformIo.Handler):
    def check(self, tag: str) -> PlatformIo.HandlerFragment | None:
        self.ignore = set()
        self.tag = tag
        self.version = packaging.version.Version(self.tag)
        for release in self.releases:
            if self.is_candidate(release):
                self.ctx.logger.info("Update available: %s/%s @ %s", self.owner, self.name, release["name"])
                return PlatformIo.HandlerFragment(
                    {
                        "version_new": release["name"],
                    }
                )
        self.ctx.logger.info("Up to date: %s/%s @ %s", self.owner, self.name, self.tag)
        return None
