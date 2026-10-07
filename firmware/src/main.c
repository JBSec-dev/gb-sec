#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#define BUTTON_PIN 2

int main(void)
{
    stdio_init_all();

    // Initialise the Pico 2 W wireless chip,
    // which also controls the onboard LED.
    if (cyw43_arch_init()) {
        return 1;
    }

    // Configure GP2 as an input.
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);

    // Hold GP2 HIGH internally while the button is not pressed.
    gpio_pull_up(BUTTON_PIN);

    while (true) {
        // Because the button connects GP2 to GND:
        // released = HIGH
        // pressed  = LOW
        bool button_pressed = !gpio_get(BUTTON_PIN);

        // Turn the onboard LED on while the button is pressed.
        cyw43_arch_gpio_put(
            CYW43_WL_GPIO_LED_PIN,
            button_pressed
        );

        sleep_ms(10);
    }
}