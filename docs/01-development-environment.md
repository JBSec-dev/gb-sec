# 01 — Development environment

## Primary environment

The planned primary environment is:

- Windows 11
- Visual Studio Code
- official Raspberry Pi Pico VS Code extension
- Raspberry Pi Pico C/C++ SDK
- Git / GitHub

Native Windows is preferred for direct hardware development because the Pico will later be accessed through USB for flashing, serial I/O, and debugging. WSL remains useful as a secondary Linux environment for security tooling and firmware analysis, but avoiding USB pass-through during early hardware work removes an unnecessary source of debugging problems.

## Target hardware

The initial target is the **Raspberry Pi Pico 2 W**, based on the RP2350 microcontroller.

The CMake board identifier used by this project is:

```text
pico2_w
```

## Build pipeline

The important distinction is that the development computer and target processor are different architectures.

```text
C source code
    |
    v
preprocessor / compiler
    |
    v
ARM object files
    |
    v
linker
    |
    +--> ELF executable (symbols + sections + machine code)
    |
    +--> UF2 / BIN firmware images
              |
              v
           RP2350
```

This is **cross-compilation**: the compiler runs on the Windows development machine but produces instructions for the RP2350's target CPU.

Later in the project, the ELF file and its memory sections will become useful security-learning material in their own right.

## Pico SDK location

The project includes the standard `pico_sdk_import.cmake` helper. It expects the Pico SDK to be discoverable through `PICO_SDK_PATH`, or it can be configured to fetch the SDK.

When using the official VS Code Pico extension, prefer the SDK/toolchain managed by the extension rather than installing multiple unrelated toolchains from older tutorials.

## Expected build outputs

A successful build should eventually produce files similar to:

```text
gb_sec.elf
gb_sec.uf2
gb_sec.bin
gb_sec.hex
```

These files are build artefacts and should not be committed to Git.

## VS Code configuration

Repository-local VS Code settings must use portable paths. Do not commit a path tied to one machine such as:

```text
/Users/name/project/...
```

The repository therefore points CMake at:

```text
${workspaceFolder}/firmware
```

This works regardless of whether the repository is cloned on Windows, macOS, or Linux.

## First pre-hardware check

Before connecting a Pico, the goal is simply:

1. configure the Pico SDK/toolchain;
2. configure CMake for `pico2_w`;
3. compile `firmware/src/main.c`;
4. confirm that a `.uf2` file is produced.

At this stage there is deliberately no display or button code. Keeping the first build minimal gives us a known-good baseline before hardware variables are introduced.

## Notes to capture during setup

Record:

- Windows version
- VS Code version
- Pico extension version
- Pico SDK version
- compiler/toolchain version
- CMake version
- exact build command or extension action used
- any setup errors and their fixes

These details make later troubleshooting reproducible.
