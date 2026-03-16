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
        if table := self._lookup_table(self._get_flash_size()):
            self.table = table
            self.project.env.BoardConfig().update("build.arduino.partitions", str(self.table))

    def _get_flash_size(self) -> str:
        size = self.project.env.GetProjectOption("board_upload.flash_size", None)
        return size if size else self.project.env.BoardConfig().get("upload.flash_size")

    def _lookup_table(self, flash_size: str) -> pathlib.Path | None:
        if flash_size in [
            "2MB",
        ]:
            return self.path / f"{flash_size}_no_ota.csv"
        elif flash_size in [
            "4MB",
        ]:
            return (
                self.path / f"{flash_size}.csv"
                if Ota.ENV_OPTION in self.project.dotenv and self.project.dotenv[Ota.ENV_OPTION] == "true"
                else self.path / f"{flash_size}_no_ota.csv"
            )
        elif flash_size in [
            "8MB",
            "16MB",
            "32MB",
        ]:
            return self.path / f"{flash_size}.csv"
        return None
