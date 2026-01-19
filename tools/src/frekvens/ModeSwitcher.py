#!/usr/bin/env python3

# Switch between modes.

import argparse
import httpx


class ModeSwitcher:
    host: str

    def __init__(self, host: str = "frekvens.local") -> None:
        self.host = host

    def set(self, mode) -> httpx.Response:
        return httpx.patch(
            f"http://{self.host}/restful/Modes", json={"mode": mode}
        ).raise_for_status()


def main() -> None:
    parser = argparse.ArgumentParser(description="Switch between modes.")
    parser.add_argument("host", help="Hostname", nargs="?", type=str)
    parser.add_argument("--mode", help="Mode", required=True, type=str)
    args = parser.parse_args()
    (ModeSwitcher(args.host) if args.host else ModeSwitcher()).set(args.mode)


if __name__ == "__main__":
    main()
