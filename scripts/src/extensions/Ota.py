import hashlib
import typing
import warnings

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
        if (
            "OTA_KEY" in self.project.dotenv
            and len(self.project.dotenv["OTA_KEY"]) != 64
        ):
            self.auth = self.project.dotenv["OTA_KEY"]
            self.project.dotenv["OTA_KEY"] = hashlib.sha256(
                self.project.dotenv["OTA_KEY"].encode()
            ).hexdigest()

    def validate(self) -> None:
        if "no_ota" in self.project.partition.table:
            if (
                self.ENV_OPTION in self.project.dotenv
                and self.project.dotenv[self.ENV_OPTION] != "false"
            ):
                warnings.warn(
                    f"{self.ENV_OPTION}: Partition table does not support OTA updates.",
                    UserWarning,
                )
            else:
                self.project.dotenv[self.ENV_OPTION] = "false"
                self.project.ota = None

    def finalize(self) -> None:
        if (
            self.project.env.GetProjectOption("upload_protocol", None) == "espota"
            and not self.project.env.GetProjectOption("upload_flags", None)
            and not self.project.env.GetProjectOption("upload_port", None)
        ):
            self.project.env.Replace(
                UPLOAD_PORT=f"{self.project.dotenv['HOSTNAME']}.local"
            )
            if self.auth:
                self.project.env.Append(
                    UPLOAD_FLAGS=[
                        f"-a={self.auth}",
                    ]
                )
