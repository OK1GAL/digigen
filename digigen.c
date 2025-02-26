#include "header.h"
 
uint64_t current_center_freq = DEFAULT_CENTER_FREQUENCY;
uint64_t current_frequency_shift = DEFAULT_FREQUENCY_SHIFT;
uint16_t current_baudrate = DEFAULT_BAUDRATE;
uint16_t current_bit_time = DEFAULT_BIT_TIME_us - SYNTH_BIT_SET_TIME_us;
uint16_t current_char_delay = (DEFAULT_BIT_TIME_us - SYNTH_BIT_SET_TIME_us) * 2;

uint8_t current_CW_speed = DEFAULT_CW_SPEED;
uint64_t current_CW_bittime_us = DEFAULT_CW_BIT_TIME_us;

uint8_t genmode = 0;
uint8_t current_drive_strenght = 0;

uint64_t setting_start_time = 0;
uint64_t setting_stop_time = 0;

uint8_t dump;

void core1_entry()
{
}

int main()
{
    stdio_init_all();
    gpio_init(ERROR_LED_PIN);
    gpio_set_dir(ERROR_LED_PIN, 1);
    gpio_init(MODE0_LED_PIN);
    gpio_set_dir(MODE0_LED_PIN, 1);
    gpio_init(MODE1_LED_PIN);
    gpio_set_dir(MODE1_LED_PIN, 1);

    gpio_init(RUNNING_LED_PIN);
    gpio_set_dir(RUNNING_LED_PIN, 1);
    gpio_init(SPACE_LED_PIN);
    gpio_set_dir(SPACE_LED_PIN, 1);
    gpio_init(MARK_LED_PIN);
    gpio_set_dir(MARK_LED_PIN, 1);


    gpio_init(PROG1_LED_PIN);
    gpio_set_dir(PROG1_LED_PIN, 1);
    gpio_init(PROG2_LED_PIN);
    gpio_set_dir(PROG2_LED_PIN, 1);
    gpio_init(PROG3_LED_PIN);
    gpio_set_dir(PROG3_LED_PIN, 1);

    gpio_init(PROGRAM_BTN_PIN);
    gpio_set_dir(PROGRAM_BTN_PIN, 0);
    gpio_init(TEST_BTN_PIN);
    gpio_set_dir(TEST_BTN_PIN, 0);
    gpio_init(RUN_FROM_EEPROM_BTN_PIN);
    gpio_set_dir(RUN_FROM_EEPROM_BTN_PIN, 0);


    gpio_put(ERROR_LED_PIN,1);
    console_init();
    gpio_put(ERROR_LED_PIN,0);

    Si5351_init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);
    set_freq(current_center_freq, SI5351_CLK0);
    multicore_launch_core1(core1_entry);

    if (i2c_read_blocking(I2C_PORT, EEPROM_ADDR, &dump,1, false))
    {
      printf("EEPROM connected\n");
    }
    else
    {
        printf("EEPROM not connected\n");
    }

    load_preset(0xff);

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
        handle_preset_switching();
        handle_run_eeprom_btn();
        gpio_put(MODE1_LED_PIN, (genmode >> 1) & 0b1);
        gpio_put(MODE0_LED_PIN, (genmode) & 0b1);
    }

    return 0;
}
