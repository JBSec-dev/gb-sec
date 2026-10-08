# Debugging log

Use this file to record faults and investigations that are worth preserving.

A useful debugging entry separates **symptoms**, **hypotheses**, **tests**, and **evidence**.

## 2026-10-08 — ST7789 display appeared dead / showed no pixel output

**Observed behaviour**

The ST7789 LCD initially looked completely black and appeared to have no backlight. After changing the ground connection, the panel became visibly backlit but still showed no pixel output.

Some Pico GPIO/GND connections also appeared not to work consistently.

**Expected behaviour**

The LCD should power up and the firmware should fill the 240x320 display red.

**Initial hypotheses**

- The LCD was not receiving 3.3 V.
- The LCD backlight pin was wired incorrectly.
- VCC/GND or the PH2.0 cable order was wrong.
- The SPI wiring or ST7789 initialisation sequence was wrong.

**Tests performed**

1. Connected an LED with a series resistor between Pico 3V3 OUT (physical pin 36) and GND. The LED lit, proving the 3.3 V rail was present.
2. Tested the same LED through the jumper paths intended for the LCD power connections.
3. Found that the test worked using physical pin 38 for GND, but not reliably through some other breadboard positions.
4. Tested GP17 / physical pin 22 as a GPIO output. The LED remained off until downward pressure was applied to the Pico.
5. This behaviour showed that the header pins were making intermittent contact with the solderless breadboard.
6. Reseated/improved the Pico-to-breadboard contact and retested the LCD.
7. The LCD then successfully displayed a full red frame.

**Root cause**

Intermittent mechanical/electrical contact between the Pico header pins and the solderless breadboard. This affected both ground and LCD control/data connections and made the fault initially look like a power or SPI problem.

**Fix**

Reseated the Pico and used connections that had been electrically verified. The LCD ground is currently connected through physical pin 38, which was confirmed working during testing.

**Lesson**

Check the physical and electrical layer before debugging a higher-level protocol. A valid firmware build and correct SPI code cannot compensate for a GPIO, ground or data pin that is not actually making contact.

The LED + resistor was useful as a simple diagnostic tool when a multimeter was not immediately available.

---

## Entry template

### YYYY-MM-DD — Short problem description

**Observed behaviour**

What happened? Record only what was actually observed.

**Expected behaviour**

What should have happened?

**Initial hypotheses**

- Hypothesis 1
- Hypothesis 2

**Tests performed**

1. Test and result.
2. Test and result.

**Root cause**

What actually caused the problem?

**Fix**

What changed?

**Lesson**

What would make this faster to diagnose next time?
