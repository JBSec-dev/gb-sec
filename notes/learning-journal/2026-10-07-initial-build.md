
**Goal**

Check to see if VS Code, the Pico Extension, CMake, Ninja, the cross-compiler, the Pico SDK, and project configurations can all producde valid RP2350 firmware. Also checked if the BOOTSEL bootloader worked on the Raspberry Pi Pico 2 W.

**What I did**

- Set up the VS Code environment, letting the Pico extentsion downloaded the needed toolchain and configurations.
- Complied a basic version of the firmware, to see the different filetypes created:
    - gb_sec.elf
    - gb_sec.elf.map
    - gb_sec.dis
    - gb_sec.bin
    - gb_sec.hex
    - gb_sec.uf2

**What I learned**

Filetypes:
    - elf: the main linked executable that has the richest output.
    - elf.map: the linker map. It tells us where code and data ended up in memory and which object/library contributed them.
    - dis: a disassembly of the firmware. It translates the generated machine instructions back into assembly-like text. 
    - bin: the raw binary firmware image.
    - hex: an Intel HEX representation of the firmware, where address/data information is stored as textual records.
    - uf2: a firmware file specifically packaged for the Pico's BOOTSEL.

- The uf2 file is a specially formatted container that holds the firmware. The machine code is wrapped inside 512-byte blocks. 

**Questions / follow-up**

- Why are all these files created and not just the uf2?
> Because UF2 is only one representation of the finished firmware, and the build system needs several other files both to construct the program and to support debugging, analysis, incremental rebuilding, and alternative programming methods.

- What are the functions of Ninja and CMake in building the firmware? 
```
                CMakeLists.txt
                    │
                    ▼
                  CMake
                    │
           "Work out how to
            build this thing"
                    │
                    ▼
               build.ninja
                    │
                    ▼
                  Ninja
                    │
           "Execute those steps"
                    │
          ┌─────────┴─────────┐
          ▼                   ▼
       compiler             linker
          │                   │
          └─────────┬─────────┘
                    ▼
                 firmware
```

---
