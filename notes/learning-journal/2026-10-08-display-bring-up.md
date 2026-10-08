**Goal**

Get the 2" ST7789 display working over SPI, understand what each of the display pins is doing, and then use the buttons to start controlling something on screen.

**What I did**

- Connected the ST7789 display to the Pico 2 W:
    - VCC -> 3V3 OUT / physical pin 36
    - GND -> physical pin 38
    - DIN -> GP19 / physical pin 25
    - CLK -> GP18 / physical pin 24
    - CS -> GP17 / physical pin 22
    - DC -> GP20 / physical pin 26
    - RST -> GP21 / physical pin 27
    - BL is currently left disconnected.
- Added `hardware_spi` to `target_link_libraries()` in CMake.
- Added a small ST7789 driver directly in `main.c` for the first bring-up.
- Configured SPI0 and the LCD control GPIO pins.
- Added functions for sending commands/data, resetting the LCD, setting an address window and filling the screen.
- The first screen test initially looked completely black/unpowered.
- Used an LED and resistor to check that physical pin 36 was actually supplying 3.3 V.
- Tested the LCD power jumper paths with the LED.
- Found that some Pico pins only worked when I physically pushed down on the Pico.
- This showed that the Pico header pins were not making reliable contact with the breadboard.
- Fixed/reseated the physical connection and got the first successful output: a full red screen.
- Changed the button test so pressing the button toggled the display between red and green.
- Added `lcd_fill_rect()` and drew a small green square on a black background.
- Made the square move across the screen while the button was held.
- Added four directional buttons:
    - GP2 = Up
    - GP3 = Down
    - GP4 = Left
    - GP5 = Right
- Used the four buttons to move the square around the display and stopped it from moving outside the screen boundaries.

**What I learned**

- SPI is being used to send bytes from the RP2350 to the ST7789 display.
    - CLK supplies the clock.
    - DIN is the display's data input, so this is connected to the Pico MOSI pin.
    - CS selects the display.
    - DC tells the display whether the bytes are commands or data.
    - RST resets the display controller.

- The display is using RGB565, so each pixel is represented by 16 bits:
    - 5 bits red
    - 6 bits green
    - 5 bits blue

- `0xF800` is red and `0x07E0` is green in RGB565.

- `lcd_set_window()` tells the ST7789 which rectangular part of the display the following pixel data belongs to. This means I do not have to redraw the whole display just to update a small square.

- The ST7789 keeps the pixels that were previously written. When moving the square I have to erase its old position with the background colour, update its coordinates, and then draw it again at the new position.

- There is a difference between detecting a button being pressed once and detecting that it is currently being held:
    - comparing the current and previous state can detect the press edge.
    - reading the current state every loop lets an object keep moving while the button is held.

- Using signed integers for the square coordinates makes moving left/up easier to handle because the values can temporarily go below zero before being clamped back to the display boundary.

- The basic program is already starting to look like a game loop:
```
input -> update state -> render -> wait -> repeat
```

- Hardware problems can look like software problems. The LCD problem originally looked like it could be power, SPI or ST7789 initialisation, but the actual problem was unreliable contact between the Pico headers and the breadboard.

**Questions / follow-up**

- Refactor the display code out of `main.c` once I understand the current functions properly.
- Create a small input layer instead of configuring/reading every button directly in the game code.
- Look at proper button debouncing instead of relying on a delay.
- Improve the frame timing instead of just using `sleep_ms(16)`.
- Add the remaining A, B, Start and Select buttons.
- Start building toward a very small game such as Pong.

---
