#pragma once

// A8PicoCart pins

#define genpin(pin) (1<<(pin))
#define genmask(low,high) ((genpin((high)-(low)+1)-1)<<(low))

#define MASK_ALL    genmask(0,29)

#define MASK_ABUS   genmask(0,12)
#define MASK_DBUS   genmask(13,20)
#define MASK_CCTL   genpin(21)
#define MASK_PHI2   genpin(22)
#define MASK_RW     genpin(23)
#define MASK_S4     genpin(24)
#define MASK_S5     genpin(25)

#define MASK_RD4    genpin(26)
#define MASK_RD5    genpin(27)

// extension, two switches
#define MASK_RAMCART_ENABLE         genpin(28)
#define MASK_RAMCART_WRITE_ENABLE   genpin(29)

#define MASK_INPUT  (genmask(0,25) | genmask(28,29))
#define MASK_OUTPUT genmask(26,27)

#define SHIFT_DBUS                  13
#define SHIFT_RAMCART_ENABLE        28
#define SHIFT_RAMCART_WRITE_ENABLE  29

#define PIN_PHI2                    22
#define PIN_RAMCART_ENABLE          28
#define PIN_RAMCART_WRITE_ENABLE    29

static inline void enable_8000_9fff(void) {
    gpio_put_masked(MASK_RD4, ~0);
}

static inline void disable_8000_9fff(void) {
    gpio_put_masked(MASK_RD4, 0);
}

static inline void enable_a000_bfff(void) {
    gpio_put_masked(MASK_RD5, ~0);
}

static inline void disable_a000_bfff(void) {
    gpio_put_masked(MASK_RD5, 0);
}

static inline void enable_8000_bfff(void) {
    gpio_put_masked(MASK_RD4|MASK_RD5, ~0);
}

static inline void disable_8000_bfff(void) {
    gpio_put_masked(MASK_RD4|MASK_RD5, 0);
}

static inline void set_databus_input(void) {
    gpio_set_dir_in_masked(MASK_DBUS);
}

static inline void set_databus_output(void) {
    gpio_set_dir_out_masked(MASK_DBUS);
}

static inline void input_wait_for_phi2_high(void) {
    while (!((input = gpio_get_all()) & MASK_PHI2)) {}
}

static inline void input_wait_for_phi2_low(void) {
    while ((input = gpio_get_all()) & MASK_PHI2) {}
}

static inline void wait_for_phi2_low(void) {
    while (gpio_get_all() & MASK_PHI2) {}
}

static inline void latch_input_on_falling_edge_phi2(void) {
    while ((input = gpio_get_all()) & MASK_PHI2) {
        latch = input;
    }
}

static inline void output_databus(uint8_t value) {
    gpio_put_masked(MASK_DBUS, ((uint32_t) value) << SHIFT_DBUS);
}

static inline bool input_cctl_is_asserted(void) {
    return !(input & MASK_CCTL);
}

static inline bool input_s4_is_asserted(void) {
    return !(input & MASK_S4);
}

static inline bool input_s5_is_asserted(void) {
    return !(input & MASK_S5);
}

static inline bool input_is_read(void) {
    return input & MASK_RW;
}

static inline bool input_ramcart_enable_switch_is_on(void) {
    return !(input & MASK_RAMCART_ENABLE);
}

static inline bool input_ramcart_write_enable_switch_is_on(void) {
    return !(input & MASK_RAMCART_WRITE_ENABLE);
}
