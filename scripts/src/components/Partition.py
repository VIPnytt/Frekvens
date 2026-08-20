import logging
import pathlib
import typing

from ..extensions.Ota import Ota

if typing.TYPE_CHECKING:
    from ..Frekvens import Frekvens


class Partition:
    path = pathlib.Path("partitions")
    project: "Frekvens"
    table: pathlib.Path

    def __init__(self, project: "Frekvens") -> None:
        self.project = project

    def initialize(self) -> None:
        if table := self.project.env.GetProjectOption("board_build.partitions", None):
            self.table = pathlib.Path(table)
            return
        if table := self._lookup_table():
            self.table = table
            self.project.env.BoardConfig().update("build.arduino.partitions", str(self.table))

    def validate(self) -> None:
        if not self.table.is_file():
            canonical = self._lookup_table()
            if canonical is not None:
                logging.warning(
                    "Partition table %r is unsupported, please use %r instead.",
                    self.table.as_posix(),
                    canonical.as_posix(),
                )

    def _lookup_table(self) -> pathlib.Path | None:
        _size = self.project.env.GetProjectOption("board_upload.flash_size", None)
        size = _size if _size else self.project.env.BoardConfig().get("upload.flash_size")
        if size == "2MB":
            return self.path / "2MB_no_ota_rev2.csv"
        elif size == "4MB":
            return (
                self.path / "4MB_rev2.csv"
                if Ota.ENV_OPTION in self.project.dotenv and self.project.dotenv[Ota.ENV_OPTION] == "true"
                else self.path / "4MB_no_ota.csv"
            )
        elif size in (
            "8MB",
            "16MB",
            "32MB",
        ):
            return self.path / f"{size}.csv"
        elif size.endswith("MB") and int(size[:-2]) >= 32:
            return self.path / "32MB.csv"
        return None
