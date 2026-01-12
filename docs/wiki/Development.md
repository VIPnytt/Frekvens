# Development

This project has some [pre-commit](https://pre-commit.com) hooks. Please activate pre-commit like this:

```bash
pip install pre-commit
pre-commit install
```

The hooks will prevent code that does not meet the project guidelines from being committed.

## Formatting and linting

The project is set up to format source files on save.

### Firmware

The default setup for the C/C++ formatter is used.

### Webapp

The default VS Code Javascript/Typescript formatter is used.

The project supports Biome for linting Typescript files. To check all files run:

```bash
cd webapp
npm install
npm run lint
```

Automatic fixes for findings are currently disabled in the script to keep commits clean.

When working on a specific file run

```bash
npx biome lint --write <./src/path/to/file>
```

to automatically fix some of the findings in that particular file.

With the [Biome extension](https://marketplace.visualstudio.com/items?itemName=biomejs.biome) for VS Code you will also get hints in the editor.
