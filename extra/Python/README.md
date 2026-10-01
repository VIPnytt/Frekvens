# 🐍 Python

Optional utilities that are not part of the core functionality but may be useful to contributors and developers.

## 🅱️ Font generator

Converts `.ttf` or `.otf` font files into C++ source files that can easily be added to the project or used as templates for adding new characters to existing fonts.

Usage:

```sh
python extra/Python/FontGenerator.py -i DejaVuSans --size 8
```

Installation of dependencies:

```sh
pip install uv
uv sync --extra font --inexact
```
