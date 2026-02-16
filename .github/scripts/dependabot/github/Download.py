import GitHub
import httpx
import packaging.version
import re


class HandlerFragment(GitHub.HandlerFragment):
    asset_new: str
    digest_new: str


class BaseFragment(HandlerFragment, GitHub.BaseFragment):
    pass


class ResultFragment(BaseFragment, GitHub.ResultFragment):
    pass


class DownloadRelease:
    ctx: GitHub.Context
    regex: re.Pattern[str]

    def __init__(self, ctx: GitHub.Context) -> None:
        self.ctx = ctx
        self.regex = re.compile(
            r"https://github\.com/(?P<owner>[^/\s]+)/(?P<repo>[^/\s]+)/releases/download/(?P<tag>[^/\s]+)/(?P<asset>[^/\s;]+)(?:\s*;\s*sha256:[0-9a-f]{64})?"
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
                    pending = Handler(self.ctx, match["owner"], match["repo"]).check(match["tag"], match["asset"])
                except (httpx.HTTPError, packaging.version.InvalidVersion):
                    self.ctx.logger.error(
                        "Update check failed: %s/%s @ %s", match["owner"], match["repo"], match["tag"], exc_info=True
                    )
                    continue
                if pending:
                    fragments.append(
                        {
                            "owner": match["owner"],
                            "repo": match["repo"],
                            "tag_old": match["tag"],
                            "tag_new": pending["tag_new"],
                            "commit_new": pending["commit_new"],
                            "asset_new": pending["asset_new"],
                            "digest_new": pending["digest_new"],
                            "string_old": string,
                        }
                    )
        return [
            ResultFragment(
                {
                    "owner": fragment["owner"],
                    "repo": fragment["repo"],
                    "tag_old": fragment["tag_old"],
                    "tag_new": fragment["tag_new"],
                    "commit_new": fragment["commit_new"],
                    "asset_new": fragment["asset_new"],
                    "digest_new": fragment["digest_new"],
                    "version_old": fragment["tag_old"].removeprefix("v"),
                    "version_new": fragment["tag_new"].removeprefix("v"),
                    "string_old": fragment["string_old"],
                    "string_new": f"{fragment['asset_new']} ; {fragment['digest_new']}",
                }
            )
            for fragment in fragments
        ]


class Handler(GitHub.Handler):
    asset: str

    def __init__(self, ctx: GitHub.Context, owner: str, repo: str) -> None:
        self.ctx = ctx
        self.owner = owner
        self.repo = repo
        self.releases = self.ctx.client.get(f"/repos/{self.owner}/{self.repo}/releases").raise_for_status().json()

    def check(self, tag: str, asset: str) -> HandlerFragment | None:
        self.asset = asset
        self.ignore = set()
        self.tag = tag
        self.version = packaging.version.Version(self.tag)
        for release in self.releases:
            if self.is_candidate(release):
                if not release["immutable"]:
                    self.ctx.logger.warning("Mutable: %s/%s @ %s", self.owner, self.repo, release["tag_name"])
                self.ctx.logger.info("Update available: %s/%s @ %s", self.owner, self.repo, release["tag_name"])
                _asset = self.parse_assets(release)
                if _asset:
                    try:
                        return HandlerFragment(
                            {
                                "asset_new": _asset["browser_download_url"],
                                "commit_new": self.ctx.client.get(
                                    f"/repos/{self.owner}/{self.repo}/commits/{release['tag_name']}"
                                )
                                .raise_for_status()
                                .json()["sha"],
                                "digest_new": _asset["digest"],
                                "tag_new": release["tag_name"],
                            }
                        )
                    except httpx.HTTPError:
                        self.ctx.logger.warning(
                            "Update failed: %s/%s @ %s", self.owner, self.repo, self.tag, exc_info=True
                        )
                    return None
                self.ctx.logger.warning("Asset not found: %s/%s @ %s", self.owner, self.repo, release["tag_name"])
                return None
        self.ctx.logger.info("Up to date: %s/%s @ %s", self.owner, self.repo, self.tag)
        return None

    def parse_assets(self, release: GitHub.ReleaseData) -> GitHub.AssetData | None:
        for asset in release["assets"]:
            if asset["name"] == self.asset.replace(
                self.tag.removeprefix("v"), release["tag_name"].removeprefix("v"), 1
            ):
                return asset
        return None
