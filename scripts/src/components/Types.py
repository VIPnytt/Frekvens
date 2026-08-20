import collections.abc
import typing

BUILD_TARGETS = globals().get("BUILD_TARGETS", [])
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
        """Retrieve an environment value by key.
        
        Returns:
        	str: The value associated with the key
        """
        raise NotImplementedError

    def AddPostAction(
        self, target: str, action: collections.abc.Callable[[list[str], list[str], "Environment"], int | None]
    ) -> None:
        """
        Register an action to run after the specified build target completes.
        
        Parameters:
        	target (str): The build target associated with the action.
        	action (collections.abc.Callable[[list[str], list[str], Environment], int | None]): A callback receiving the target name, command arguments, and environment.
        """
        raise NotImplementedError

    def Append(self, **kwargs: typing.Any) -> None:
        """Appends values to environment variables.
        
        Parameters:
            **kwargs (typing.Any): Variable names mapped to values to append.
        """
        raise NotImplementedError

    def BoardConfig(self) -> BoardConfig:
        raise NotImplementedError

    def GetProjectConfig(self) -> ProjectConfig:
        raise NotImplementedError

    def GetProjectOption(self, option: str, default: typing.Any = None) -> typing.Any:
        raise NotImplementedError

    def IsCleanTarget(self) -> bool:
        raise NotImplementedError

    def Replace(self, **kwargs: typing.Any) -> None:
        raise NotImplementedError

    def StringifyMacro(self, value: str) -> str:
        raise NotImplementedError

    def subst(self, key: str) -> str:
        raise NotImplementedError


def Import(*vars: str) -> None:
    raise NotImplementedError
