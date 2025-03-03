// PicoRAMCart -- Emulate Raster/Bob!k/C.P.U. RAMCart with A8PicoCart
//
// Copyright (C) 2025 by Ivo van Poorten
// BSD-2 License. See LICENSE file for details
//
// Hardware needs two switches between GPIO28 and GND, and GPIO29 and GND

#include "optimize.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/clocks.h"
#include "hardware/structs/bus_ctrl.h"

static uint32_t input, latch;
static uint16_t address;
static uint8_t data;

#include "picocart.h"

#define NUMBANKS    16              // must be power of 2
#define LASTBANK    (NUMBANKS-1)
#define BANKMASK    LASTBANK

static uint8_t banking_register;
uint8_t memory[NUMBANKS][8192];

void __not_in_flash_func(run_core0)(void) {
    while(1) {
        input_wait_for_phi2_high();

        if (input_ramcart_enable_switch_is_on()) {
            enable_8000_bfff();
            if (input_cctl_is_asserted()) {     // control
                if (input_is_read()) {
                    set_databus_output();
                    output_databus(banking_register);
                    wait_for_phi2_low();
                    set_databus_input();
                    continue;
                } else {
                    latch_input_on_falling_edge_phi2();
                    data = (latch & MASK_DBUS) >> SHIFT_DBUS;
                    banking_register = data & BANKMASK;
                    continue;
                }
            } else {                            // memory banks
                int bank = -1;
                if (input_s4_is_asserted()) {
                    bank = banking_register;
                } else if (input_s5_is_asserted()) {
                    bank = LASTBANK;
                }
                if (bank >= 0) {
                    address = input & MASK_ABUS;
                    if (input_is_read()) {
                        set_databus_output();
                        output_databus(memory[bank][address]);
                        wait_for_phi2_low();
                        set_databus_input();
                        continue;
                    } else {
                        if (input_ramcart_write_enable_switch_is_on()) {
                            latch_input_on_falling_edge_phi2();
                            data = (latch & MASK_DBUS) >> SHIFT_DBUS;
                            memory[bank][address] = data;
                            continue;
                        }
                    }
                }
            }
        } else {
            disable_8000_bfff();
        }
        wait_for_phi2_low();
    }
}

void run_core1(void) {
    while (1) {}
}

int main(void) {
    gpio_init(PIN_PHI2);
    gpio_set_dir(PIN_PHI2, GPIO_IN);

    while (to_ms_since_boot(get_absolute_time()) < 100) {
        if (gpio_get(PIN_PHI2)) {

            // a8picocart
            gpio_init_mask(MASK_ALL);
            gpio_set_dir_in_masked(MASK_INPUT);
            gpio_set_dir_out_masked(MASK_OUTPUT);

            disable_8000_bfff();

            // pullup ramcart switches
            gpio_pull_up(PIN_RAMCART_ENABLE);
            gpio_pull_up(PIN_RAMCART_WRITE_ENABLE);

            set_sys_clock_khz(250000, true);

            // multicore_launch_core1(run_core1);
            // bus_ctrl_hw->priority = BUSCTRL_BUS_PRIORITY_PROC0_BITS;
            run_core0();
        }
    }

    stdio_init_all();
    while (1) {
        printf("Set baudrate to 1200 to enter bootsel mode\n");
        sleep_ms(100);
        printf("On Linux, run stty -F /dev/ttyACM0 1200\n");
        sleep_ms(100);
    }
}
