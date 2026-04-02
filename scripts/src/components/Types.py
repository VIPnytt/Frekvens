import typing

COMMAND_LINE_TARGETS = globals().get("COMMAND_LINE_TARGETS", [])


class BoardConfig:
    def get(self, key: str, default: typing.Any = None):
        raise NotImplementedError

    def update(self, key: str, value: typing.Any) -> None:
        raise NotImplementedError


class ProjectConfig:
    def get(self, section: str, option: str, default: typing.Any = None):
        raise NotImplementedError


class Environment:
    def __getitem__(self, key: str) -> str:
        raise NotImplementedError

    def Append(self, **kwargs: typing.Any) -> None:
        raise NotImplementedError

    def BoardConfig(self) -> BoardConfig:
        raise NotImplementedError

    def GetProjectConfig(self) -> ProjectConfig:
        raise NotImplementedError

    def GetProjectOption(self, option: str, default: typing.Any = None) -> typing.Any:
        raise NotImplementedError

    def IsCleanTarget(self) -> bool:
        raise NotImplementedError

    def StringifyMacro(self, value: str) -> str:
        raise NotImplementedError


def Import(*vars: str) -> None:
    raise NotImplementedError
