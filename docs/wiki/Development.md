# 👷 Development

## 🚧 Formatting and linting

### Firmware

Clang-format is being used for all C/C++ code.

### Tools and scripts

Ruff is being used for all Python code.

### Webapp

Biome is being used for linting and formatting of Typescript files.

Install dependencies:

```sh
cd webapp
npm install
```

Check all files:

```sh
cd webapp
npx biome check ./src
```

Reformat or apply linter fixes to specific files:

```sh
cd webapp
npx biome format --write <./src/path/to/file>
npx biome lint --write <./src/path/to/file>
```
