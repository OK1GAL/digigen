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

uint8_t current_custom_text[32] = "RYRYRYRYRYRYRYRYRYRYRYRYRYRYRYRY";
uint8_t current_custom_text_length = 32;

uint8_t beaconen = 0;

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

    gpio_init(TTY_TX_PIN);
    gpio_set_dir(TTY_TX_PIN, 1);
    gpio_init(TTY_RX_PIN);
    gpio_set_dir(TTY_RX_PIN, 0);

    gpio_put(ERROR_LED_PIN, 1);
    console_init();
    gpio_put(ERROR_LED_PIN, 0);

    Si5351_init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);
    set_freq(current_center_freq, SI5351_CLK0);
    multicore_launch_core1(core1_entry);

    if (i2c_read_blocking(I2C_PORT, EEPROM_ADDR, &dump, 1, false))
    {
        set_console_color(CONSOLE_DGREEN_COLOR);
        printf("EEPROM connected\n");
        set_console_color(CONSOLE_RESET_COLOR);
    }
    else
    {
        set_console_color(CONSOLE_DRED_COLOR);
        printf("EEPROM not connected\n");
        set_console_color(CONSOLE_RESET_COLOR);
    }

    load_preset(0xff);

    print_current_config();
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

void print_current_config()
{
    set_console_color(CONSOLE_DMAGENTA_COLOR);
    printf("Current config:\n");
    printf("Mode: \033[1;37m%d\033[1;35m\n", genmode);
    printf("0:Simple carier 1:CW 2:RTTY 3:HELL\n");
    printf("Drive strenght: \033[1;37m%d\033[1;35m   |0 = 2mA 1 = 4mA 2 = 6mA 3 = 8mA\n", current_drive_strenght);
    printf("Custom text: \033[1;37m");
    for (int i = 0; i < current_custom_text_length; i++)
    {
        printf("%c", current_custom_text[i]);
    }
    printf("\033[1;35m\n");
    printf("Custom text length: \033[1;37m%d\033[1;35m\n", current_custom_text_length);
    printf("Center frequency: \033[1;37m%llu Hz\033[1;35m \n", current_center_freq);
    printf("RTTY:\n");
    printf("Frequency shift: \033[1;37m%llu Hz\033[1;35m \n", current_frequency_shift);
    printf("Baudrate: \033[1;37m%u Bd\033[1;35m \n", current_baudrate);
    printf("CW:\n");
    printf("CW speed: \033[1;37m%d WPM\033[1;35m \n", current_CW_speed);
    printf("HELL:\n");
    printf("Hell speed: \033[1;37m%d Bd\033[1;35m \n", current_HELL_speed);
    set_console_color(CONSOLE_RESET_COLOR);
}