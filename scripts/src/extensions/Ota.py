import hashlib
import logging
import typing

if typing.TYPE_CHECKING:
    from ..Frekvens import Frekvens


class Ota:
    ENV_OPTION: str = "EXTENSION_OTA"
    NAME: str = "OTA"
    project: "Frekvens"
    auth: str | None = None

    def __init__(self, project: "Frekvens") -> None:
        self.project = project

    def configure(self) -> None:
        if "OTA_KEY" in self.project.dotenv and len(self.project.dotenv["OTA_KEY"]) != 64:
            self.auth = self.project.dotenv["OTA_KEY"]
            self.project.dotenv["OTA_KEY"] = hashlib.sha256(self.project.dotenv["OTA_KEY"].encode()).hexdigest()

    def validate(self) -> None:
        if "no_ota" in str(self.project.partition.table):
            if self.ENV_OPTION in self.project.dotenv and self.project.dotenv[self.ENV_OPTION] != "false":
                logging.error("%s: Partition table does not support %s updates.", self.ENV_OPTION, self.NAME)
            else:
                self.project.dotenv[self.ENV_OPTION] = "false"
                self.project.ota = None
        if (
            self.ENV_OPTION not in self.project.dotenv or self.project.dotenv[self.ENV_OPTION] != "true"
        ) and self.project.env.GetProjectOption("upload_protocol", None) == "espota":
            logging.warning(
                "%s: %s is disabled. Future uploads will require wired flashing.", self.ENV_OPTION, self.NAME
            )

    def finalize(self) -> None:
        if (
            self.project.env.GetProjectOption("upload_protocol", None) == "espota"
            and not self.project.env.GetProjectOption("upload_flags", None)
            and not self.project.env.GetProjectOption("upload_port", None)
        ):
            self.project.env.Replace(UPLOAD_PORT=f"{self.project.dotenv['HOSTNAME']}.local")
            if self.auth:
                self.project.env.Append(
                    UPLOAD_FLAGS=[
                        f"-a={self.auth}",
                    ]
                )
