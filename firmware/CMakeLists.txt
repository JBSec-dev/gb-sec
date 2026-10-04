cmake_minimum_required(VERSION 3.13)

# Target the Raspberry Pi Pico 2 W (RP2350 + wireless module).
set(PICO_BOARD pico2_w CACHE STRING "Target Pico board")

# Standard Raspberry Pi helper for locating/importing the Pico SDK.
include(pico_sdk_import.cmake)

project(gb_sec C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(gb_sec
    src/main.c
)

target_link_libraries(gb_sec
    pico_stdlib
)

# USB serial is useful during development; UART can be enabled later when needed.
pico_enable_stdio_usb(gb_sec 1)
pico_enable_stdio_uart(gb_sec 0)

target_compile_options(gb_sec PRIVATE
    -Wall
    -Wextra
)

# Produce .uf2, .bin, .hex and other useful firmware outputs.
pico_add_extra_outputs(gb_sec)
