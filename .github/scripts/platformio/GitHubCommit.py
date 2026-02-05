import datetime
import httpx
import json
import logging
import packaging.version
import re
import typing


class GitHubCommit:
    major: int = 90
    minor: int = 21
    patch: int = 3
    path: str = "platformio.ini"
    rest: httpx.Client

    def __init__(self) -> None:
        self.rest = httpx.Client(
            base_url="https://api.github.com",
            headers={
                "Accept": "application/vnd.github+json",
                "User-Agent": "Frekvens (+https://github.com/VIPnytt/Frekvens)",
                "X-GitHub-Api-Version": "2022-11-28",
            },
        )

    def parse(self) -> str:
        seen: set[str] = set()
        fragments: list[typing.Tuple[str, str, str, str, str, str, str]] = []
        with open(self.path, encoding="utf-8") as f:
            for match in re.finditer(
                r"https://github\.com/(?P<owner>[^/]+)/(?P<repo>[^/]+)/archive/(?P<commit>[0-9a-f]{40})\.tar\.gz\s*;\s*(?P<tag>.+)",
                f.read(),
            ):
                string = match.group(0)
                if string in seen:
                    logging.debug("Skipping duplicate: %s", string)
                    continue
                seen.add(string)
                package = match.groupdict()
                pending = self.check(package["owner"], package["repo"], package["commit"], package["tag"])
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

    def check(self, owner: str, repo: str, commit: str, tag: str) -> typing.Optional[typing.Tuple[str, str]]:
        logging.info("Checking for updates to %s/%s %s", owner, repo, tag)
        try:
            v_tag = packaging.version.Version(tag)
            for release in self.rest.get(f"/repos/{owner}/{repo}/releases").raise_for_status().json():
                try:
                    v_release = packaging.version.Version(release["tag_name"])
                    if v_release < v_tag:
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
                        logging.info("Skipping '%s', days since release: %d", release["tag_name"], days)
                        continue
                    commits = (
                        self.rest.get(f"/repos/{owner}/{repo}/commits/{release['tag_name']}").raise_for_status().json()
                    )
                    if commits["sha"] == commit:
                        logging.info("Up to date")
                        return None
                    return (commits["sha"], release["tag_name"])
                except (httpx.HTTPError, packaging.version.InvalidVersion) as e:
                    logging.warning(e)
        except (httpx.HTTPError, packaging.version.InvalidVersion) as e:
            logging.error(e)
        return None


def main() -> None:
    print(GitHubCommit().parse())


if __name__ == "__main__":
    main()
