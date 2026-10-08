# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.3.1)
set(toolchainVersion 15_2_Rel1)
set(picotoolVersion 2.3.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.13)

# Target the Raspberry Pi Pico 2 W (RP2350 + wireless module).
set(PICO_BOARD pico2_w CACHE STRING "Target Pico board")

# Standard Raspberry Pi helper for locating/importing the Pico SDK.
include(pico_sdk_import.cmake)

# Declares the project and its supported languages (C, C++, and assembly).
project(gb_sec C CXX ASM)

# Set the C and C++ standards to be used for compiling the project.
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK. This must be called before any other SDK functions.
pico_sdk_init()

# Add the main executable target for the project, specifying the source files to be compiled.
add_executable(gb_sec
    src/main.c
)

# Link the Pico standard library to the executable target, providing essential functions and utilities for the Raspberry Pi Pico.
target_link_libraries(gb_sec
    pico_stdlib
    pico_cyw43_arch_none
    hardware_spi
)

# USB serial is useful during development; UART can be enabled later when needed.
pico_enable_stdio_usb(gb_sec 1)
pico_enable_stdio_uart(gb_sec 0)

# Set compiler options for the target, enabling all warnings and extra warnings to help catch potential issues during development.
target_compile_options(gb_sec PRIVATE
    -Wall
    -Wextra
)

# Produce .uf2, .bin, .hex and other useful firmware outputs.
pico_add_extra_outputs(gb_sec)
