#include "header.h"

uint64_t current_center_freq = DEFAULT_CENTER_FREQUENCY;
uint64_t current_frequency_shift = DEFAULT_FREQUENCY_SHIFT;
uint16_t current_baudrate = DEFAULT_BAUDRATE;
uint16_t current_bit_time = DEFAULT_BIT_TIME_us - SYNTH_BIT_SET_TIME_us;
uint16_t current_char_delay = (DEFAULT_BIT_TIME_us - SYNTH_BIT_SET_TIME_us) * 2;

uint8_t current_CW_speed = DEFAULT_CW_SPEED;
uint64_t current_CW_bittime_us = DEFAULT_CW_BIT_TIME_us;

uint8_t genmode = 0;

uint64_t setting_start_time = 0;
uint64_t setting_stop_time = 0;

void core1_entry()
{
}

int main()
{
    stdio_init_all();
    gpio_init(MODE0_LED_PIN);
    gpio_set_dir(MODE0_LED_PIN, 1);
    gpio_init(MODE1_LED_PIN);
    gpio_set_dir(MODE1_LED_PIN, 1);
    console_init();

    Si5351_init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);
    set_freq(current_center_freq, SI5351_CLK0);
    // recalculate_fsk_params(current_center_freq, current_frequency_shift);
    // set_FSK_symbol(1);
    multicore_launch_core1(core1_entry);

    printf("Current config:\n");
    printf("Mode: %d\n", genmode);
    printf("0:Simple carier 1:CW 2:RTTY\n");
    printf("Center frequency: %lluHz\n", current_center_freq);
    printf("RTTY:\n");
    printf("Frequency shift: %lluHz\n", current_frequency_shift);
    printf("Baudrate: %u baud\n", current_baudrate);
    printf("CW:\n");
    printf("CW speed: %dWPM\n", current_CW_speed);
    printf("\n");
    printf("--");

    while (true)
    {
        handle_console_rx();

        gpio_put(MODE1_LED_PIN, (genmode >> 1) & 0b1);
        gpio_put(MODE0_LED_PIN, (genmode) & 0b1);
    }

    return 0;
}
