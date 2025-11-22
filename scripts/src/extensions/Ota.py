import hashlib
import typing

if typing.TYPE_CHECKING:
    from ..Project import Project


class Ota:
    ENV_OPTION: str = "EXTENSION_OTA"
    NAME: str = "OTA"
    project: "Project"
    _auth: str | None

    def __init__(self, project: "Project") -> None:
        self.project = project

    def configure(self) -> None:
        if (
            "OTA_KEY" in self.project.dotenv
            and len(self.project.dotenv["OTA_KEY"]) != 64
        ):
            self._auth = self.project.dotenv["OTA_KEY"]
            self.project.dotenv["OTA_KEY"] = hashlib.sha256(
                self.project.dotenv["OTA_KEY"].encode()
            ).hexdigest()

    def validate(self) -> None:
        if "no_ota" in self.project.partition.table:
            if (
                self.ENV_OPTION in self.project.dotenv
                and self.project.dotenv[self.ENV_OPTION] != "false"
            ):
                UserWarning(f"{self.ENV_OPTION}: Partition table does not support OTA.")
            else:
                self.project.dotenv[self.ENV_OPTION] = "false"
                self.project.ota = None

    def finalize(self) -> None:
        config = self.project.env.GetProjectConfig()
        if (
            config.has_option(self.project.working, "upload_protocol")
            and not config.has_option(self.project.working, "upload_flags")
            and not config.has_option(self.project.working, "upload_port")
            and self.project.env.GetProjectOption("upload_protocol") == "espota"
        ):
            self.project.env.Replace(
                UPLOAD_PORT=f"{self.project.dotenv['HOSTNAME']}.local"
            )
            if self._auth is not None:
                self.project.env.Append(
                    UPLOAD_FLAGS=[
                        f"-a={self._auth}",
                    ]
                )
