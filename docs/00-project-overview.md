# 00 — Project overview

## What is GB-Sec?

GB-Sec is a handheld games-console project designed primarily as a learning platform.

The first version is a breadboard prototype using a Raspberry Pi Pico 2 W, an SPI LCD, and physical buttons. The project will then grow in stages: first into a small games platform, then into a custom embedded system, and finally into a hardware-security lab containing both intentionally insecure and hardened designs.

The objective is not simply to produce a working handheld. The objective is to understand the layers that make it work.

## Why build it?

This project combines several areas that are interesting independently but especially useful together:

- embedded C/C++
- digital electronics
- microcontroller peripherals
- computer architecture
- graphics and game-loop design
- debugging and instrumentation
- firmware analysis
- secure boot and software authenticity
- hardware attack surfaces

The project is also intended to become a portfolio piece. Documentation therefore needs to capture not only the final design but the reasoning, experiments, mistakes, and lessons that led to it.

## Initial design

The first prototype uses:

| Component | Initial choice | Reason |
| --- | --- | --- |
| MCU board | Raspberry Pi Pico 2 W | RP2350 is powerful, inexpensive, well documented, and has security features worth exploring later |
| Display | 2" ST7789 SPI IPS LCD | Common controller, simple digital interface, suitable resolution for a handheld |
| Controls | Momentary tactile switches | Easy to prototype on breadboard and map directly to GPIO |
| Power | USB during development | Removes battery/power-management variables while debugging |
| Language | C/C++ with Pico SDK | Gives direct exposure to embedded concepts and the firmware build process |

## Planned development phases

### Phase 0 — environment and repository

Establish the build toolchain, repository structure, documentation process, and a minimal firmware target.

### Phase 1 — basic hardware I/O

Learn GPIO by reading buttons and controlling simple outputs. Bring up the ST7789 display over SPI and draw basic colours/shapes.

### Phase 2 — game-console foundations

Create small abstractions for input and graphics, then implement a fixed-timestep game loop and a simple game such as Pong.

### Phase 3 — storage, audio, and launcher

Add persistent/external storage, sound, and a basic game-selection interface.

### Phase 4 — custom runtime

Experiment with a small executable or bytecode format so that games are data loaded by the console rather than hard-coded into one firmware image.

### Phase 5 — insecure security model

Add intentionally weak game/firmware validation, debug services, or device-to-device protocols. Document how and why they fail.

### Phase 6 — hardening

Introduce authenticated software, stronger key handling, secure-boot concepts, interface restrictions, and a formal threat model.

### Phase 7 — custom hardware

Move from development modules to a custom PCB and enclosure.

## Engineering principles

1. **Understand before abstracting.** Use libraries where sensible, but understand the important boundaries: GPIO, SPI, memory, boot flow, and security decisions.
2. **Change one thing at a time.** Small experiments make hardware faults easier to isolate.
3. **Record failures.** A failed hypothesis is still useful engineering evidence.
4. **Keep commits meaningful.** Git history should tell the development story.
5. **Separate observation from assumption.** Documentation should distinguish what was measured from what was expected.
6. **Do not make security claims without a threat model.** A feature is not "secure" merely because it uses cryptography.

## Current milestone status

### Phase 0 — complete

- Reproducible Pico 2 W build environment established.
- VS Code Pico extension, CMake, Ninja, compiler/linker and Pico SDK build path tested.
- BOOTSEL flashing confirmed.
- Generated build artefacts kept out of Git.

### Phase 1 — in progress

The first physical I/O and display milestones are now working:

- button input using GPIO and internal pull-ups;
- Pico 2 W onboard LED control through CYW43;
- ST7789 display bring-up over SPI0;
- RGB565 full-screen colour fills;
- rectangular region drawing;
- four-direction button input;
- a small square that can be moved around the screen while remaining inside the display boundaries.

The next step is to turn the bring-up code into small input/display abstractions and then build toward a simple game loop and Pong.
