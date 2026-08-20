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
        table = self.project.env.GetProjectOption("board_build.partitions", None)
        self.table = pathlib.Path(table) if table else self._lookup_table()
        self.project.env.BoardConfig().update("build.arduino.partitions", str(self.table))

    def validate(self) -> None:
        if not self.table.is_file():
            logging.warning(
                "Partition table %r is unsupported, please use %r instead.",
                self.table.as_posix(),
                self._lookup_table().as_posix(),
            )

    def _lookup_table(self) -> pathlib.Path:
        flash_size = (
            self.project.env.GetProjectOption("board_upload.flash_size", None)
            or self.project.env.BoardConfig().get("upload.flash_size", None)
            or "4MB"
        )
        size = int(flash_size[:-2]) if flash_size.endswith("MB") else 4
        if size <= 2:
            return self.path / "2MB_no_ota_rev2.csv"
        elif size <= 4:
            return (
                self.path / "4MB_rev2.csv"
                if Ota.ENV_OPTION in self.project.dotenv and self.project.dotenv[Ota.ENV_OPTION] == "true"
                else self.path / "4MB_no_ota.csv"
            )
        return self.path / f"{min(32, 2 ** (size.bit_length() - 1))}MB.csv"
