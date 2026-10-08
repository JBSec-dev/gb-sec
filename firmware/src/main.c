#include <stdint.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/spi.h"

// Buttons
#define BUTTON_UP    2
#define BUTTON_DOWN  3
#define BUTTON_LEFT  4
#define BUTTON_RIGHT 5

// LCD dimensions
#define LCD_WIDTH  240
#define LCD_HEIGHT 320

// Colour changes
#define COLOUR_RED 0xF800
#define COLOUR_GREEN 0x07E0

// We are using SPI0
#define LCD_SPI spi0

// LCD pin assignments
#define LCD_CS   17
#define LCD_CLK  18
#define LCD_MOSI 19
#define LCD_DC   20
#define LCD_RST  21
#define LCD_BL   22


static void lcd_write_command(uint8_t command)
{
    // DC LOW means the byte is a command.
    gpio_put(LCD_DC, 0);

    // CS LOW selects the display.
    gpio_put(LCD_CS, 0);

    spi_write_blocking(LCD_SPI, &command, 1);

    // CS HIGH deselects the display.
    gpio_put(LCD_CS, 1);
}


static void lcd_write_data(const uint8_t *data, size_t length)
{
    // DC HIGH means the bytes are data.
    gpio_put(LCD_DC, 1);

    gpio_put(LCD_CS, 0);

    spi_write_blocking(LCD_SPI, data, length);

    gpio_put(LCD_CS, 1);
}


static void lcd_reset(void)
{
    // Hardware reset sequence.
    gpio_put(LCD_RST, 1);
    sleep_ms(10);

    gpio_put(LCD_RST, 0);
    sleep_ms(20);

    gpio_put(LCD_RST, 1);
    sleep_ms(120);
}


static void lcd_init(void)
{
    // Start SPI0 at 20 MHz.
    spi_init(LCD_SPI, 20 * 1000 * 1000);

    // Connect RP2350 GPIO pins to the SPI peripheral.
    gpio_set_function(LCD_CLK, GPIO_FUNC_SPI);
    gpio_set_function(LCD_MOSI, GPIO_FUNC_SPI);

    // Explicitly configure:
    // 8 bits per transfer
    // clock idle LOW
    // sample on first edge
    // MSB first
    spi_set_format(
        LCD_SPI,
        8,
        SPI_CPOL_0,
        SPI_CPHA_0,
        SPI_MSB_FIRST
    );

    // Configure the control pins as ordinary GPIO outputs.
    gpio_init(LCD_CS);
    gpio_set_dir(LCD_CS, GPIO_OUT);

    gpio_init(LCD_DC);
    gpio_set_dir(LCD_DC, GPIO_OUT);

    gpio_init(LCD_RST);
    gpio_set_dir(LCD_RST, GPIO_OUT);

    gpio_init(LCD_BL);
    gpio_set_dir(LCD_BL, GPIO_OUT);

    // Initial states.
    gpio_put(LCD_CS, 1);
    gpio_put(LCD_DC, 1);
    gpio_put(LCD_RST, 1);
    gpio_put(LCD_BL, 0);

    lcd_reset();

    // Software reset.
    lcd_write_command(0x01);
    sleep_ms(150);

    // Sleep Out.
    lcd_write_command(0x11);
    sleep_ms(120);

    // Colour mode: 16 bits per pixel (RGB565).
    lcd_write_command(0x3A);
    uint8_t colour_mode = 0x55;
    lcd_write_data(&colour_mode, 1);

    // Memory access control.
    lcd_write_command(0x36);
    uint8_t memory_access = 0x00;
    lcd_write_data(&memory_access, 1);

    // Display inversion on.
    lcd_write_command(0x21);

    // Normal display mode.
    lcd_write_command(0x13);

    // Display on.
    lcd_write_command(0x29);
    sleep_ms(100);

    // Finally enable the physical backlight.
    gpio_put(LCD_BL, 1);
}


static void lcd_set_window(
    uint16_t x0,
    uint16_t y0,
    uint16_t x1,
    uint16_t y1
)
{
    uint8_t data[4];

    // Column Address Set
    lcd_write_command(0x2A);

    data[0] = x0 >> 8;
    data[1] = x0 & 0xFF;
    data[2] = x1 >> 8;
    data[3] = x1 & 0xFF;

    lcd_write_data(data, 4);

    // Row Address Set
    lcd_write_command(0x2B);

    data[0] = y0 >> 8;
    data[1] = y0 & 0xFF;
    data[2] = y1 >> 8;
    data[3] = y1 & 0xFF;

    lcd_write_data(data, 4);

    // Memory Write
    lcd_write_command(0x2C);
}


static void lcd_fill(uint16_t colour)
{
    lcd_set_window(
        0,
        0,
        LCD_WIDTH - 1,
        LCD_HEIGHT - 1
    );

    // RGB565 is 16 bits = 2 bytes per pixel.
    uint8_t pixel_buffer[256];

    for (size_t i = 0; i < sizeof(pixel_buffer); i += 2) {
        pixel_buffer[i]     = colour >> 8;
        pixel_buffer[i + 1] = colour & 0xFF;
    }

    uint32_t pixels_remaining = LCD_WIDTH * LCD_HEIGHT;

    // We are now writing pixel data.
    gpio_put(LCD_DC, 1);
    gpio_put(LCD_CS, 0);

    while (pixels_remaining > 0) {

        // Our buffer contains 128 pixels.
        uint32_t pixels_this_time =
            pixels_remaining > 128
                ? 128
                : pixels_remaining;

        spi_write_blocking(
            LCD_SPI,
            pixel_buffer,
            pixels_this_time * 2
        );

        pixels_remaining -= pixels_this_time;
    }

    gpio_put(LCD_CS, 1);
}

static void lcd_fill_rect(
    uint16_t x,
    uint16_t y,
    uint16_t width,
    uint16_t height,
    uint16_t colour
)
{
    // Ignore rectangles completely outside the display.
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT) {
        return;
    }

    // Clip the rectangle if it extends beyond the screen.
    if (x + width > LCD_WIDTH) {
        width = LCD_WIDTH - x;
    }

    if (y + height > LCD_HEIGHT) {
        height = LCD_HEIGHT - y;
    }

    lcd_set_window(
        x,
        y,
        x + width - 1,
        y + height - 1
    );

    uint8_t pixel_buffer[256];

    for (size_t i = 0; i < sizeof(pixel_buffer); i += 2) {
        pixel_buffer[i]     = colour >> 8;
        pixel_buffer[i + 1] = colour & 0xFF;
    }

    uint32_t pixels_remaining = width * height;

    gpio_put(LCD_DC, 1);
    gpio_put(LCD_CS, 0);

    while (pixels_remaining > 0) {

        uint32_t pixels_this_time =
            pixels_remaining > 128
                ? 128
                : pixels_remaining;

        spi_write_blocking(
            LCD_SPI,
            pixel_buffer,
            pixels_this_time * 2
        );

        pixels_remaining -= pixels_this_time;
    }

    gpio_put(LCD_CS, 1);
}

int main(void)
{
    stdio_init_all();

    if (cyw43_arch_init()) {
        return 1;
    }

    gpio_init(BUTTON_UP);
    gpio_set_dir(BUTTON_UP, GPIO_IN);
    gpio_pull_up(BUTTON_UP);

    gpio_init(BUTTON_DOWN);
    gpio_set_dir(BUTTON_DOWN, GPIO_IN);
    gpio_pull_up(BUTTON_DOWN);

    gpio_init(BUTTON_LEFT);
    gpio_set_dir(BUTTON_LEFT, GPIO_IN);
    gpio_pull_up(BUTTON_LEFT);

    gpio_init(BUTTON_RIGHT);
    gpio_set_dir(BUTTON_RIGHT, GPIO_IN);
    gpio_pull_up(BUTTON_RIGHT);

    lcd_init();

    const uint16_t background_colour = 0x0000;
    const uint16_t square_colour = 0x07E0;

    const uint16_t square_size = 40;
    const uint16_t move_speed = 2;

    int square_x = 100;
    int square_y = 140;

    // Draw the initial scene.
    lcd_fill(background_colour);

    lcd_fill_rect(
        square_x,
        square_y,
        square_size,
        square_size,
        square_colour
    );

    while (true) {

    bool up_pressed    = !gpio_get(BUTTON_UP);
    bool down_pressed  = !gpio_get(BUTTON_DOWN);
    bool left_pressed  = !gpio_get(BUTTON_LEFT);
    bool right_pressed = !gpio_get(BUTTON_RIGHT);

    int old_x = square_x;
    int old_y = square_y;

    if (up_pressed) {
        square_y -= move_speed;
    }

    if (down_pressed) {
        square_y += move_speed;
    }

    if (left_pressed) {
        square_x -= move_speed;
    }

    if (right_pressed) {
        square_x += move_speed;
    }

    // Keep the square inside the display.
    if (square_x < 0) {
        square_x = 0;
    }

    if (square_y < 0) {
        square_y = 0;
    }

    if (square_x + square_size > LCD_WIDTH) {
        square_x = LCD_WIDTH - square_size;
    }

    if (square_y + square_size > LCD_HEIGHT) {
        square_y = LCD_HEIGHT - square_size;
    }

    // Only redraw if the square actually moved.
    if (square_x != old_x || square_y != old_y) {

        lcd_fill_rect(
            old_x,
            old_y,
            square_size,
            square_size,
            background_colour
        );

        lcd_fill_rect(
            square_x,
            square_y,
            square_size,
            square_size,
            square_colour
        );
    }

    bool any_button_pressed =
        up_pressed ||
        down_pressed ||
        left_pressed ||
        right_pressed;

    cyw43_arch_gpio_put(
        CYW43_WL_GPIO_LED_PIN,
        any_button_pressed
    );

    sleep_ms(16);
    }
}