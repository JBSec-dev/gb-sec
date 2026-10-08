# GB-Sec

GB-Sec is a learning-focused handheld game console project built around the Raspberry Pi Pico 2 W (RP2350).

The project starts as a simple embedded C/C++ games console and will gradually evolve into a hardware-security lab for exploring topics such as firmware loading, debug interfaces, signed software, secure boot, protocol design, and embedded attack surfaces.

## Current status

**Phase 1 — basic hardware I/O and display bring-up**

The first breadboard prototype is running. The Pico 2 W can read physical buttons, drive the onboard LED, communicate with the 240x320 ST7789 display over SPI, draw RGB565 colours/rectangles, and move a small square around the screen using four directional buttons.

The next step is to refactor the working bring-up code into small display/input components and then build toward a simple game such as Pong.

## Initial hardware

- Raspberry Pi Pico 2 W with headers
- 2" SPI IPS LCD using an ST7789 controller
- Momentary tactile push buttons
- Breadboard and jumper wires
- USB power

Later revisions are expected to add storage, audio, a custom PCB, battery power, and deliberately vulnerable/hardened security features.

## Project goals

- Learn embedded C/C++ by building something tangible.
- Understand GPIO, SPI, interrupts, memory, timers, DMA, and other microcontroller fundamentals.
- Design a small graphics/input layer and eventually a game runtime.
- Learn hardware debugging with serial, SWD, and logic-analysis tools.
- Apply cybersecurity concepts to firmware and embedded systems.
- Keep an engineering record detailed enough to support a future technical write-up and portfolio discussion.

## Repository layout

```text
gb-sec/
├── firmware/        # RP2350 firmware and build configuration
├── docs/            # Structured project documentation
├── hardware/        # Hardware notes, pinouts, and later schematics/PCB files
├── security/        # Threat models, security experiments, and mitigations
├── notes/           # Learning journal and debugging log
└── images/          # Project photographs and diagrams
```

## Development philosophy

The project is intentionally incremental. Important code should be understood rather than copied blindly, and failed experiments should be documented alongside successful ones.

So far the prototype has progressed through GPIO input, display communication, RGB565 graphics primitives and a basic input/update/render loop. The project will continue to build these pieces up gradually rather than introducing a large framework before the underlying behaviour is understood.

## Build

The intended primary development environment is native Windows with VS Code and the official Raspberry Pi Pico extension. WSL may later be used for security tooling and analysis, but direct hardware development will remain native where practical.

Detailed setup notes live in [docs/01-development-environment.md](docs/01-development-environment.md).

## Documentation

Start with:

- [Project overview](docs/00-project-overview.md)
- [Development environment](docs/01-development-environment.md)
- [Learning journal](notes/learning-journal/)
- [Debugging log](notes/debugging-log.md)
- [Hardware notes and current pin assignments](hardware/README.md)

## Licence

A project licence has not yet been selected.
