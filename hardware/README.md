# Hardware

This directory contains hardware-specific documentation for GB-Sec.

## Initial prototype

The first prototype uses development modules rather than a custom PCB:

- Raspberry Pi Pico 2 W with pre-soldered headers
- 2" 240x320 ST7789 SPI IPS display
- tactile push buttons
- solderless breadboard
- jumper wires
- USB power

## Current prototype pin assignments

These are prototype assignments and may change as the hardware develops.

### Direction buttons

The buttons use the Pico's internal pull-up resistors and connect the GPIO to GND when pressed, so they are active-low.

| Control | GPIO | Physical pin |
| --- | ---: | ---: |
| Up | GP2 | 4 |
| Down | GP3 | 5 |
| Left | GP4 | 6 |
| Right | GP5 | 7 |

Planned later controls remain GP6-GP9 for A, B, Start and Select.

### ST7789 display

| LCD pin | Pico connection | Physical pin |
| --- | --- | ---: |
| VCC | 3V3 OUT | 36 |
| GND | GND | 38 |
| DIN / MOSI | GP19 / SPI0 TX | 25 |
| CLK / SCK | GP18 / SPI0 SCK | 24 |
| CS | GP17 | 22 |
| DC | GP20 | 26 |
| RST | GP21 | 27 |
| BL | currently disconnected | - |

The display backlight is currently operating with the BL wire disconnected, so firmware control of the backlight has been deferred until later.

Physical pin 38 is currently used for LCD ground because it was verified during bring-up. Other Pico GND pins are electrically common, but unreliable breadboard/header contact caused misleading results during the first display test.

## Proven hardware milestones

- Pico 2 W BOOTSEL/USB flashing works.
- Onboard CYW43-controlled LED works.
- GPIO button input works using internal pull-ups.
- ST7789 display communication over SPI0 works.
- Full-screen RGB565 colour fills work.
- Rectangular region updates work.
- Four directional buttons can move a rendered square around the display.

## Planned contents

As the project develops, this directory will hold:

- pin assignments
- wiring diagrams
- component notes
- power-budget notes
- schematics
- PCB design files
- enclosure measurements
- bring-up checklists

The initial prototype deliberately avoids battery power and audio so that early debugging focuses on one subsystem at a time.
