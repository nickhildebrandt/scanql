# scanql
A lightweight SQL validator written in C.

## Tools for Compiling and Testing
- meson
- ninja
- gcc or clang

## Setup Meson Build
```bash
meson setup build
```
**Setup for release build:**
```bash
meson setup build --buildtype=release
```

## Compile the Code
```bash
meson compile -C build
```

## Execute the build
```bash
./build/src/scanql <SQL-String>
```

## Run Tests
```bash
meson test -C build
```

## Just format the code
```bash
meson compile -C build format
```

## Adding source Files
To add new source files, simply place them in the `src/` directory and update
the `meson.build` file in the same directory to include the new files.
