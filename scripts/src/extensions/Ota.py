import hashlib
import logging
import typing

if typing.TYPE_CHECKING:
    from ..components.Types import COMMAND_LINE_TARGETS
    from ..Frekvens import Frekvens
else:
    from SCons.Script import COMMAND_LINE_TARGETS


class Ota:
    ENV_OPTION: typing.Final[str] = "EXTENSION_OTA"
    NAME: typing.Final[str] = "OTA"
    project: "Frekvens"
    auth: str | None

    def __init__(self, project: "Frekvens") -> None:
        """Initialize the OTA manager with a project and no authentication key."""
        self.auth = None
        self.project = project

    def configure(self) -> None:
        """
        Configure the OTA authentication key for project use.
        
        Stores the original key for upload authentication and replaces the project key with its SHA-256 hexadecimal digest when it is not already a 64-character hash.
        """
        if "OTA_KEY" in self.project.dotenv and len(self.project.dotenv["OTA_KEY"]) != 64:
            self.auth = self.project.dotenv["OTA_KEY"]
            self.project.dotenv["OTA_KEY"] = hashlib.sha256(self.project.dotenv["OTA_KEY"].encode()).hexdigest()

    def validate(self) -> None:
        """
        Validate OTA support against the selected partition table and upload protocol.
        """
        if "no_ota" in self.project.partition.table.name:
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
        """
        Configure OTA upload settings, including the protocol, host-based port, and authentication flags.
        """
        protocol = self.project.env.GetProjectOption("upload_protocol", None)
        if protocol != "espota" and any(target.endswith("ota") for target in COMMAND_LINE_TARGETS):
            protocol = "espota"
            self.project.env.Replace(UPLOAD_PROTOCOL=protocol)
        if (
            protocol == "espota"
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
