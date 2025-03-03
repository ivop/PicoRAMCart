cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_FETCH_FROM_GIT on)

include(pico_sdk_import.cmake)

project(PicoRAMCart)

pico_sdk_init()

add_executable(PicoRAMCart
    PicoRAMCart.c
)

pico_enable_stdio_usb(PicoRAMCart 1)
pico_enable_stdio_uart(PicoRAMCart 0)

target_compile_definitions(PicoRAMCart PRIVATE PICO_FLASH_SIZE_BYTES=16777216)

target_link_libraries(PicoRAMCart PUBLIC pico_stdlib pico_multicore)

pico_add_extra_outputs(PicoRAMCart)
