import GitHub
import httpx
import packaging.version
import re


class TagArchive:
    ctx: GitHub.Context
    regex: re.Pattern[str]

    def __init__(self, ctx: GitHub.Context) -> None:
        self.ctx = ctx
        self.regex = re.compile(
            r"https://github\.com/(?P<owner>[^/\s]+)/(?P<repo>[^/\s]+)/archive/refs/tags/(?P<tag>[^/\s]+)\.(?:tar\.gz|zip)(?:\s*;\s*[0-9a-f]{7,40})?"
        )

    def matrix(self) -> list[GitHub.ResultFragment]:
        seen: set[str] = set()
        fragments: list[GitHub.BaseFragment] = []
        with open(self.ctx.workspace / "platformio.ini", encoding="utf-8") as f:
            for match in self.regex.finditer(f.read()):
                string = match.group(0)
                if string in seen:
                    continue
                seen.add(string)
                try:
                    pending = Handler(self.ctx, match["owner"], match["repo"]).check(match["tag"])
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
                            "string_old": string,
                        }
                    )
        return [
            GitHub.ResultFragment(
                {
                    "owner": fragment["owner"],
                    "repo": fragment["repo"],
                    "tag_old": fragment["tag_old"],
                    "tag_new": fragment["tag_new"],
                    "commit_new": fragment["commit_new"],
                    "version_old": fragment["tag_old"].removeprefix("v"),
                    "version_new": fragment["tag_new"].removeprefix("v"),
                    "string_old": fragment["string_old"],
                    "string_new": f"https://github.com/{fragment['owner']}/{fragment['repo']}/archive/refs/tags/{fragment['tag_new']}.tar.gz ; {fragment['commit_new'][:7]}",
                }
            )
            for fragment in fragments
        ]


class TagBall:
    ctx: GitHub.Context
    regex: re.Pattern[str]

    def __init__(self, ctx: GitHub.Context) -> None:
        self.ctx = ctx
        self.regex = re.compile(
            r"https://api\.github\.com/repos/(?P<owner>[^/\s]+)/(?P<repo>[^/\s]+)/(?:tar|zip)ball/(?P<tag>[^/\s;]+)(?:\s*;\s*[0-9a-f]{7,40})?"
        )

    def matrix(self) -> list[GitHub.ResultFragment]:
        seen: set[str] = set()
        fragments: list[GitHub.BaseFragment] = []
        with open(self.ctx.workspace / "platformio.ini", encoding="utf-8") as f:
            for match in self.regex.finditer(f.read()):
                string = match.group(0)
                if string in seen:
                    continue
                seen.add(string)
                if len(match["tag"]) == 40 and all("0" <= char <= "9" or "a" <= char <= "f" for char in match["tag"]):
                    continue
                try:
                    pending = Handler(self.ctx, match["owner"], match["repo"]).check(match["tag"])
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
                            "string_old": string,
                        }
                    )
        return [
            GitHub.ResultFragment(
                {
                    "owner": fragment["owner"],
                    "repo": fragment["repo"],
                    "tag_old": fragment["tag_old"],
                    "tag_new": fragment["tag_new"],
                    "commit_new": fragment["commit_new"],
                    "version_old": fragment["tag_old"].removeprefix("v"),
                    "version_new": fragment["tag_new"].removeprefix("v"),
                    "string_old": fragment["string_old"],
                    "string_new": f"https://api.github.com/repos/{fragment['owner']}/{fragment['repo']}/tarball/{fragment['tag_new']} ; {fragment['commit_new'][:7]}",
                }
            )
            for fragment in fragments
        ]


class Handler(GitHub.Handler):
    def __init__(self, ctx: GitHub.Context, owner: str, repo: str) -> None:
        self.ctx = ctx
        self.owner = owner
        self.repo = repo
        self.releases = self.ctx.client.get(f"/repos/{self.owner}/{self.repo}/releases").raise_for_status().json()

    def check(self, tag: str) -> GitHub.HandlerFragment | None:
        self.ignore = set()
        self.tag = tag
        self.version = packaging.version.Version(self.tag)
        for release in self.releases:
            if self.is_candidate(release):
                if not release["immutable"]:
                    self.ctx.logger.warning("Mutable: %s/%s @ %s", self.owner, self.repo, release["tag_name"])
                self.ctx.logger.info("Update available: %s/%s @ %s", self.owner, self.repo, release["tag_name"])
                try:
                    return GitHub.HandlerFragment(
                        {
                            "commit_new": self.ctx.client.get(
                                f"/repos/{self.owner}/{self.repo}/commits/{release['tag_name']}"
                            )
                            .raise_for_status()
                            .json()["sha"],
                            "tag_new": release["tag_name"],
                        }
                    )
                except httpx.HTTPError:
                    self.ctx.logger.warning("Update failed: %s/%s @ %s", self.owner, self.repo, self.tag, exc_info=True)
                    return None
        self.ctx.logger.info("Up to date: %s/%s @ %s", self.owner, self.repo, self.tag)
        return None
