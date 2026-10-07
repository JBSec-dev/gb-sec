**Goal**

Check to see if VS Code, the Pico Extension, CMake, Ninja, the cross-compiler, the Pico SDK, and project configurations can all produce valid RP2350 firmware. Also checked if the BOOTSEL bootloader worked on the Raspberry Pi Pico 2 W.

After proving the build process worked, I wanted to get the first basic input/output working on the physical hardware by blinking the onboard LED and then controlling it with a push button connected to a GPIO pin.

**What I did**

- Set up the VS Code environment, letting the Pico extension download the needed toolchain and configurations.
- Put the Pico 2 W into BOOTSEL mode and confirmed that Windows mounted it as `RP2350 (D:)`.
- Compiled a basic version of the firmware, to see the different filetypes created:
    - gb_sec.elf
    - gb_sec.elf.map
    - gb_sec.dis
    - gb_sec.bin
    - gb_sec.hex
    - gb_sec.uf2
- Changed `main.c` to initialise the CYW43 chip and blink the Pico 2 W onboard LED every 500 ms.
- Updated `CMakeLists.txt` to link `pico_cyw43_arch_none`, as the LED is controlled through the CYW43 wireless chip rather than directly from an RP2350 GPIO.
- Flashed the new `gb_sec.uf2` file using BOOTSEL and confirmed the LED blinked on and off.
- Put the Pico 2 W onto the breadboard.
- Connected a tactile button between GP2 (physical pin 4) and GND (physical pin 38).
- Changed the firmware so that pressing the button turns the onboard LED on, and releasing it turns the LED off.

**What I learned**

Filetypes:
    - elf: the main linked executable that has the richest output.
    - elf.map: the linker map. It tells us where code and data ended up in memory and which object/library contributed them.
    - dis: a disassembly of the firmware. It translates the generated machine instructions back into assembly-like text.
    - bin: the raw binary firmware image.
    - hex: an Intel HEX representation of the firmware, where address/data information is stored as textual records.
    - uf2: a firmware file specifically packaged for the Pico's BOOTSEL.

- The uf2 file is a specially formatted container that holds the firmware. The machine code is wrapped inside 512-byte blocks.

- CYW43 is a wireless microcontroller driver and system-on-chip (SoC) interface used to control the Infineon CYW43439 Wi-Fi and Bluetooth chip found on boards like the Raspberry Pi Pico W / Pico 2 W. The built-in LED light on the Pico 2 W was used to conduct a "blinking test". This LED is part of the wireless chip, hence the reason to use CYW43 rather than directly controlling it through a normal RP2350 GPIO.

- Including a header file such as `pico/cyw43_arch.h` gives the compiler the declarations for the CYW43 functions, but the implementation still has to be linked into the executable. This is why `pico_cyw43_arch_none` was also added to `target_link_libraries()` in CMake.

- A GPIO pin can be configured as an input or output. For the button test, GP2 was configured as an input.

- The button is wired between GP2 and GND. An internal pull-up resistor keeps GP2 at a known HIGH state while the button is not pressed. Without the pull-up, the input could float and give unpredictable readings.

- The button is active-low:
    - released = HIGH / 1
    - pressed = LOW / 0

- In the code, `!gpio_get(BUTTON_PIN)` reverses this so that the variable `button_pressed` is `true` when the physical button is actually pressed.

- Physical pin numbers and GPIO numbers are not the same thing. GP2 is GPIO number 2, but it is physical pin 4 on the Pico header.

- The first full input/output path is now working:
```
button -> GP2 -> RP2350 firmware -> CYW43 -> onboard LED
```

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

- Next I want to connect the ST7789 display and understand how SPI works, including SCK, MOSI, CS, DC, RESET and the backlight pin.

---
