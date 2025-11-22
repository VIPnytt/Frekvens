import typing

from ..extensions.Ota import Ota

if typing.TYPE_CHECKING:
    from ..Project import Project


class Partition:
    project: "Project"
    table: str

    def __init__(self, project: "Project") -> None:
        self.project = project

    def initialize(self) -> None:
        if self.project.env.GetProjectConfig().has_option(
            self.project.working, "board_build.partitions"
        ):
            self.table = self.project.env.GetProjectOption("board_build.partitions")
        else:
            _table = self._lookup_table(self._get_flash_size())
            if _table is not None:
                self.table = _table
                self.project.env.BoardConfig().update(
                    "build.arduino.partitions", self.table
                )

    def _get_flash_size(self) -> str:
        board_config = self.project.env.BoardConfig()
        if self.project.env.GetProjectConfig().has_option(
            self.project.working, "board_upload.flash_size"
        ):
            return self.project.env.GetProjectOption("board_upload.flash_size")
        return board_config.get("upload.flash_size")

    def _lookup_table(self, flash_size) -> str | None:
        if flash_size in [
            "2MB",
        ]:
            return f"partitions/{flash_size}_no_ota.csv"
        elif flash_size in [
            "4MB",
        ]:
            return (
                f"partitions/{flash_size}.csv"
                if Ota.ENV_OPTION in self.project.dotenv
                and self.project.dotenv[Ota.ENV_OPTION] == "true"
                else f"partitions/{flash_size}_no_ota.csv"
            )
        elif flash_size in [
            "8MB",
            "16MB",
            "32MB",
        ]:
            return f"partitions/{flash_size}.csv"
        return None
