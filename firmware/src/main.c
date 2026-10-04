#include "pico/stdlib.h"

int main(void)
{
    // Initialise the Pico SDK's standard I/O subsystem.
    // USB serial is enabled in CMake for future diagnostic output.
    stdio_init_all();

    // The first milestone is intentionally a minimal, valid firmware image.
    // Hardware-specific behaviour will be added incrementally.
    while (true) {
        tight_loop_contents();
    }
}
