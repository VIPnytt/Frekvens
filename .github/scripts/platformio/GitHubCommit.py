import datetime
import httpx
import json
import logging
import os
import packaging.version
import re
import typing


class GitHubCommit:
    logger: logging.Logger
    major: int = 90
    minor: int = 21
    patch: int = 3
    rest: httpx.Client

    def __init__(self) -> None:
        self.logger = logging.getLogger("Frekvens")
        self.rest = httpx.Client(
            base_url="https://api.github.com",
            headers={
                "Accept": "application/vnd.github+json",
                "Authorization": f"Bearer {os.environ.get('GITHUB_TOKEN')}",
                "User-Agent": "Frekvens (+https://github.com/VIPnytt/Frekvens)",
                "X-GitHub-Api-Version": "2022-11-28",
            },
        )

    def matrix(self) -> str:
        seen: set[str] = set()
        fragments: list[typing.Tuple[str, str, str, str, str, str, str]] = []
        with open("platformio.ini", encoding="utf-8") as f:
            for match in re.finditer(
                r"https://github\.com/(?P<owner>[^/]+)/(?P<repo>[^/]+)/archive/(?P<commit>[0-9a-f]{40})\.tar\.gz\s*;\s*(?P<tag>.+)",
                f.read(),
            ):
                string = match.group(0)
                if string in seen:
                    continue
                seen.add(string)
                package = match.groupdict()
                pending = self.check(package["owner"], package["repo"], package["tag"])
                if pending:
                    fragments.append(
                        (
                            package["owner"],
                            package["repo"],
                            package["commit"],
                            package["tag"],
                            *pending,
                            string,
                        )
                    )
        return json.dumps(
            [
                {
                    "owner": owner,
                    "repo": repo,
                    "tag_old": tag_old,
                    "commit_old": commit_old,
                    "string_old": string_old,
                    "version_old": tag_old.removeprefix("v"),
                    "tag_new": tag_new,
                    "commit_new": commit_new,
                    "string_new": f"https://github.com/{owner}/{repo}/archive/{commit_new}.tar.gz ; {tag_new}",
                    "version_new": tag_new.removeprefix("v"),
                }
                for owner, repo, commit_old, tag_old, commit_new, tag_new, string_old in fragments
            ]
        )

    def check(self, owner: str, repo: str, tag: str) -> typing.Optional[typing.Tuple[str, str]]:
        try:
            v_tag = packaging.version.Version(tag)
            for release in self.rest.get(f"/repos/{owner}/{repo}/releases").raise_for_status().json():
                try:
                    v_release = packaging.version.Version(release["tag_name"])
                    if v_release <= v_tag:
                        continue
                    elif v_release.is_prerelease:
                        self.logger.info("Skipping pre-release: %s/%s %s", owner, repo, release["tag_name"])
                        continue
                    days = (
                        datetime.datetime.now(datetime.timezone.utc)
                        - datetime.datetime.fromisoformat(release["published_at"].replace("Z", "+00:00"))
                    ).days
                    if (
                        (days < self.major and v_release.major > v_tag.major)
                        or (days < self.minor and v_release.minor > v_tag.minor and v_release.major == v_tag.major)
                        or (days < self.patch and v_release.minor == v_tag.minor and v_release.major == v_tag.major)
                    ):
                        self.logger.info("On cooldown, skipping: %s/%s %s", owner, repo, release["tag_name"])
                        continue
                    self.logger.info("Update available: %s/%s %s", owner, repo, release["tag_name"])
                    return (
                        self.rest.get(f"/repos/{owner}/{repo}/commits/{release['tag_name']}")
                        .raise_for_status()
                        .json()["sha"],
                        release["tag_name"],
                    )
                except (httpx.HTTPError, packaging.version.InvalidVersion) as e:
                    self.logger.warning(e)
        except (httpx.HTTPError, packaging.version.InvalidVersion) as e:
            self.logger.error(e)
        self.logger.info("Up to date: %s/%s %s", owner, repo, tag)
        return None


def main() -> None:
    logging.basicConfig()
    logging.getLogger("Frekvens").setLevel(logging.INFO)
    print(GitHubCommit().matrix())


if __name__ == "__main__":
    main()
