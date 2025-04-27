#include "header.h"

#define MIN_BAUDRATE 5   // vytazeno z prdele
#define MAX_BAUDRATE 300 // theoreticaly 1kBaud

#define MIN_CFREQ 10000     // vytazeno z prdele
#define MAX_CFREQ 200000000 // vytazeno z prdele

#define MIN_SFREQ 5     // vytazeno z prdele
#define MAX_SFREQ 10000 // vytazeno z prdele

#define CONSOLE_INPUT_BUF_SIZE 32
uint8_t console_input_buf[CONSOLE_INPUT_BUF_SIZE];

void handle_console_command();
uint8_t compare_command(const char *command, uint8_t size);
uint8_t is_number(uint8_t start, uint8_t size);
uint8_t string_to_uint8(uint8_t start);
uint64_t string_to_uint64(uint8_t start);
uint16_t string_to_uint16(uint8_t start);

void console_init()
{
    while (!stdio_usb_connected())
    {
        busy_wait_ms(1);
        if (time_us_64() > STDIO_WAIT_TIME_us)
        {
            break;
        }
    }
    set_console_color(CONSOLE_YELLOW_COLOR);
    printf("\n\n\n\nWelcome to digigen!\n");
    set_console_color(CONSOLE_RESET_COLOR);
}

int8_t c = 0;
int8_t dc = 0;
uint8_t counter = 0;
void handle_console_rx()
{
    c = getchar_timeout_us(0);
    if (c != PICO_ERROR_TIMEOUT)
    {
        if (c == 8 && counter > 0) // check for backspace and if backspace is posible
        {
            // handle backspace
            counter--;
            printf("%c %c", 8, 8);
        }
        else if (c == 10 || c == 13) // check for linefeed
        {
            /* printf("\ncommand:\n");
            for (uint8_t i = 0; i < counter; i++)
            {
                printf("%c", console_input_buf[i]);
            }
            printf("\nsize: %d\n", counter); */
            printf("\n");
            handle_console_command();
            counter = 0;
            c = 0;
            for (int i = 0; i < CONSOLE_INPUT_BUF_SIZE; i++)
            {
                console_input_buf[i] = 0;
            }

            printf("--");
            while (getchar_timeout_us(0) != PICO_ERROR_TIMEOUT)
            {
                /* code */
            }
            return;
        }
        else if (counter < CONSOLE_INPUT_BUF_SIZE && c > 31 && c < 127) // check if buffer isn't full and handle input characters
        {
            printf("%c", c);
            console_input_buf[counter] = c;
            counter++;
        }
        else
        {
            // buffer is probably full or character is invalid, so do nothing
            busy_wait_ms(1);
        }
        c = 0;
    }
}

void handle_console_command()
{
    if (compare_command("help", 4))
    {
        set_console_color(CONSOLE_DCYAN_COLOR);
        printf("###digigen help###\n");
        printf("all commands have to have the same format as below\n");
        printf("help - prints help\n");
        printf("config - prints current config\n");
        printf("baud xxx - sets baudrate example for 50 baud:\"baud 050\"\n");
        printf("cfreq xxxxxxxxx - sets center frequency in Hz, example for 7100kHz:\"cfreq 007100000\"\n");
        printf("sfreq xxxxxxxxx - sets shift frequency in Hz, example for 125Hz:\"sfreq 000000125\"\n");
        printf("  |     |     |\n");
        printf("  |     |     |\n");
        printf("-----------------\n");
        printf("  |<-s->c<-s->|\n");
        printf("c: center freq, s: shift\n");
        printf("cwspeed xx - sets CW speed in WPM\n");
        printf("txen x - sets the tx state for example: \"txen 1\"\n");
        printf("manrttytx - starts manual rtty transmision to escape use ctrl+c\n");
        printf("txmode x - sets TX mode 0:carier 1:CW 2:RTTY\n");
        printf("mancwtx = starts manual CW transmition, escape using ctrl+c\n");
        printf("savep x - saves current config to a preset x.\n");
        printf("loadp x - loads config from preset x.\n");
        printf("drives x - sets the SI5351 drive strenght 0=2mA 1=4mA 2=6mA 3=8mA.\n");
        printf("default - memory formating\n");
        printf("hellspeed xxx - set speed of hell transmition (default 125)\n");
        printf("manhelltx - starts manual hell transmition to escape use ctrl+c\n");
        set_console_color(CONSOLE_RESET_COLOR);
    }
    else if (compare_command("config", 6))
    {
        print_current_config();
    }
    else if (compare_command("baud ", 5))
    {
        uint16_t baud_to_set = string_to_uint16(5);
        if (baud_to_set >= MIN_BAUDRATE && baud_to_set <= MAX_BAUDRATE)
        {
            set_baudrate(baud_to_set);
            set_console_color(CONSOLE_DGREEN_COLOR);
            printf("Baudrate set to: %u baud\n", current_baudrate);
            printf("Bit time: %u us\n", current_bit_time + SYNTH_BIT_SET_TIME_us);
            set_console_color(CONSOLE_RESET_COLOR);
            switch (genmode)
            {
            case 0:
                // TODO
                break;
            case 1:
                refresh_CW_config();
                break;
            case 2:
                refresh_RTTY_config();
                break;
            case 3:
                refresh_HELL_config();
                break;

            default:
                break;
            }
            save_current_to_preset(0xff); // save to last config
        }
        else
        {
            set_console_color(CONSOLE_DRED_COLOR);
            printf("Baudrate invalid %d < baud rate < %d\n", MIN_BAUDRATE, MAX_BAUDRATE);
            set_console_color(CONSOLE_RESET_COLOR);
        }
    }
    else if (compare_command("cfreq ", 6))
    {

        uint64_t cfreq_to_set = string_to_uint64(6);
        if (cfreq_to_set >= MIN_CFREQ && cfreq_to_set <= MAX_CFREQ)
        {
            current_center_freq = cfreq_to_set;
            set_console_color(CONSOLE_DGREEN_COLOR);
            printf("Center frequency set to: %lluHz\n", current_center_freq);
            set_console_color(CONSOLE_RESET_COLOR);
            switch (genmode)
            {
            case 0:
                // TODO
                break;
            case 1:
                refresh_CW_config();
                break;
            case 2:
                refresh_RTTY_config();
                break;
            case 3:
                refresh_HELL_config();
                break;

            default:
                break;
            }
            save_current_to_preset(0xff); // save to last config
        }
        else
        {
            set_console_color(CONSOLE_DRED_COLOR);
            printf("Frequency invalid, %d < cfreq < %d\n", MIN_CFREQ, MAX_CFREQ);
            set_console_color(CONSOLE_RESET_COLOR);
        }
    }
    else if (compare_command("sfreq ", 6))
    {

        uint64_t sfreq_to_set = string_to_uint64(6);
        if (sfreq_to_set >= MIN_SFREQ && sfreq_to_set <= MAX_SFREQ)
        {
            current_frequency_shift = sfreq_to_set;
            set_console_color(CONSOLE_DGREEN_COLOR);
            printf("Shift frequency set to: %lluHz\n", current_frequency_shift);
            set_console_color(CONSOLE_RESET_COLOR);
            switch (genmode)
            {
            case 0:
                // TODO
                break;
            case 1:
                refresh_CW_config();
                break;
            case 2:
                refresh_RTTY_config();
                break;
            case 3:
                refresh_HELL_config();
                break;

            default:
                break;
            }
            save_current_to_preset(0xff); // save to last config
        }
        else
        {
            set_console_color(CONSOLE_DRED_COLOR);
            printf("Frequency invalid, %d < sfreq < %d\n", MIN_SFREQ, MAX_SFREQ);
            set_console_color(CONSOLE_RESET_COLOR);
        }
    }
    else if (compare_command("cwspeed ", 8))
    {

        uint64_t cwspeed = string_to_uint64(8);
        if (cwspeed > 0 && cwspeed < 100)
        {
            current_CW_speed = cwspeed;
            set_CW_WPM(current_CW_speed);
            set_console_color(CONSOLE_DGREEN_COLOR);
            printf("CW speed set to: %d\n", current_CW_speed);
            set_console_color(CONSOLE_RESET_COLOR);
            save_current_to_preset(0xff); // save to last config
        }
        else
        {
            set_console_color(CONSOLE_DRED_COLOR);
            printf("Invalid speed, min = 1, max = 100\n");
            set_console_color(CONSOLE_RESET_COLOR);
        }
    }
    else if (compare_command("txen ", 5))
    {

        uint8_t txen = string_to_uint8(5);
        if (txen == 0)
        {
            tx_enable(0);
            set_console_color(CONSOLE_YELLOW_COLOR);
            printf("TX disabled\n");
            set_console_color(CONSOLE_RESET_COLOR);
        }
        else
        {
            tx_enable(1);
            set_console_color(CONSOLE_YELLOW_COLOR);
            printf("TX enabled\n");
            set_console_color(CONSOLE_RESET_COLOR);
        }
    }
    else if (compare_command("manrttytx", 9))
    {
        set_console_color(CONSOLE_RESET_COLOR);
        if (genmode == 2)
        {
            int8_t c = ' ';
            RTTYTXletter(c, 1);
            gpio_put(RUNNING_LED_PIN, 1);
            while (c != 3)
            {
                c = getchar_timeout_us(0);
                if (c != PICO_ERROR_TIMEOUT)
                {
                    if (c == 10 || c == 13)
                    {
                        printf("\n");
                        RTTYTXletter(10, 0);
                        busy_wait_us(current_char_delay * 5);
                        RTTYTXletter(13, 0);
                    }
                    else
                    {
                        printf("%c", c);
                        RTTYTXletter(c, 0);
                    }

                    busy_wait_us(1000000 / current_baudrate);
                }
                else
                {
                    busy_wait_ms(1);
                }
            }
            gpio_put(RUNNING_LED_PIN, 0);
            printf("\n", c);
        }
        else
        {
            set_console_color(CONSOLE_DRED_COLOR);
            printf("ERROR: incorect mode selected.\n");
            set_console_color(CONSOLE_RESET_COLOR);
        }
    }
    else if (compare_command("txmode ", 7))
    {
        uint8_t mode = string_to_uint8(7);
        if (mode >= 0 && mode <= GENMODE_MAX)
        {
            genmode = mode;
            switch (genmode)
            {
            case 0:
                // TODO
                break;
            case 1:
                set_CW_mode();
                break;
            case 2:
                set_RTTY_mode();
                break;
            case 3:
                set_HELL_mode();
                break;

            default:
                break;
            }
            save_current_to_preset(0xff); // save to last config
            set_console_color(CONSOLE_DGREEN_COLOR);
            printf("Mode set to %d\n", genmode);
            set_console_color(CONSOLE_RESET_COLOR);
        }
        else
        {
            set_console_color(CONSOLE_DRED_COLOR);
            printf("ERROR: invalid mode, 0:Simple carier 1:CW 2:RTTY 3:HELL\n");
            set_console_color(CONSOLE_RESET_COLOR);
        }
    }
    else if (compare_command("mancwtx", 7))
    {
        if (genmode == 1)
        {
            int8_t c = ' ';
            CW_TX_letter(c);
            gpio_put(RUNNING_LED_PIN, 1);
            while (c != 3)
            {
                c = getchar_timeout_us(0);
                if (c != PICO_ERROR_TIMEOUT)
                {

                    if (c == 10 || c == 13)
                    {
                        printf("\n");
                    }
                    else
                    {
                        printf("%c", c);
                        CW_TX_letter(c);
                    }
                }
                else
                {
                    busy_wait_ms(1);
                }
            }
            gpio_put(RUNNING_LED_PIN, 0);
            printf("\n", c);
        }
        else
        {
            set_console_color(CONSOLE_DRED_COLOR);
            printf("ERROR: incorect mode selected.\n");
            set_console_color(CONSOLE_RESET_COLOR);
        }
    }
    else if (compare_command("savep ", 6))
    {

        uint8_t save_location = string_to_uint8(6);
        if (save_current_to_preset(save_location))
        {
            set_console_color(CONSOLE_DGREEN_COLOR);
            printf("Preset saved.\n");
            set_console_color(CONSOLE_RESET_COLOR);
        }
        else
        {
            set_console_color(CONSOLE_DRED_COLOR);
            printf("Preset save unsuccesfull, try different preset location.\n");
            set_console_color(CONSOLE_RESET_COLOR);
        }
    }
    else if (compare_command("loadp ", 6))
    {
        uint8_t load_location = string_to_uint8(6);
        if (load_preset(load_location))
        {
            set_console_color(CONSOLE_DGREEN_COLOR);
            printf("Preset loaded.\n");
            set_console_color(CONSOLE_RESET_COLOR);
        }
        else
        {
            set_console_color(CONSOLE_DRED_COLOR);
            printf("Preset load unsuccesfull, try different preset location.\n");
            set_console_color(CONSOLE_RESET_COLOR);
        }
    }
    else if (compare_command("drives ", 7))
    {
        uint8_t drive_strenght = string_to_uint8(7);
        if (drive_strenght < 4)
        {
            set_console_color(CONSOLE_DGREEN_COLOR);
            set_tx_drive_strenght(drive_strenght);
            printf("Drive strenght set to ");
            set_console_color(CONSOLE_DBLUE_COLOR);
            switch (drive_strenght)
            {
            case 0:
                printf("2mA.\n");
                break;
            case 1:
                printf("4mA.\n");
                break;
            case 2:
                printf("6mA.\n");
                break;
            case 3:
                printf("8mA.\n");
                break;

            default:
            set_console_color(CONSOLE_DRED_COLOR);
                printf(". Huh, this isn't right.\n");
                break;
            }
            set_console_color(CONSOLE_RESET_COLOR);
            save_current_to_preset(0xff); // save to last config
        }
        else
        {
            set_console_color(CONSOLE_DRED_COLOR);
            printf("Invalid drive strenght, 0 = 2mA 1 = 4mA 2 = 6mA 3 = 8mA.\n");
            set_console_color(CONSOLE_RESET_COLOR);
        }
    }
    else if (compare_command("default", 7))
    {
        set_console_color(CONSOLE_YELLOW_COLOR);
        printf("You are about to put your digigen to default config,\n");
        printf("are you sure you want to do that? [Y/N]\n");
        printf("If ou want to keep calibration data and only default\n");
        printf("presets you can do so by pressing [P].\n");
        c = getchar_timeout_us(0);
        while (true)
        {
            c = getchar_timeout_us(0);
            if (c == 'Y' || c == 'y')
            {
                set_console_color(CONSOLE_DRED_COLOR);
                printf("Reconfiguring memory, please wait.\n");
                default_memory();
                set_console_color(CONSOLE_DGREEN_COLOR);
                printf("Default config loaded.\n");
                break;
            }
            else if (c == 'P' || c == 'p')
            {
                set_console_color(CONSOLE_DRED_COLOR);
                printf("Reconfiguring memory, please wait.\n");
                initialize_memory();
                set_console_color(CONSOLE_DGREEN_COLOR);
                printf("Default presets loaded.\n");
                break;
            }
            else if (c == 'N' || c == 'n')
            {
                set_console_color(CONSOLE_YELLOW_COLOR);
                printf("Nothing was done.\n");
                break;
            }
        }
        set_console_color(CONSOLE_RESET_COLOR);
    }
    else if (compare_command("settext", 7))
    {
        c = getchar_timeout_us(0);
        uint8_t message[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        uint8_t message_length = 0;
        while (true)
        {
            c = getchar_timeout_us(0);
            if (c != PICO_ERROR_TIMEOUT)
            {
                if (c == 8 && message_length > 0) // check for backspace and if backspace is posible
                {
                    // handle backspace
                    message_length--;
                    printf("%c %c", 8, 8);
                }
                else if (c == 3)
                {
                    set_console_color(CONSOLE_DRED_COLOR);
                    printf("Message wasn't saved.");
                    set_console_color(CONSOLE_RESET_COLOR);
                    message_length = 0;
                    c = 0;
                    printf("--");
                    while (getchar_timeout_us(0) != PICO_ERROR_TIMEOUT)
                    {
                        /* code */
                    }
                    break;
                }
                else if (c == 10 || c == 13) // check for linefeed
                {

                    for (int i = 0; i < message_length; i++)
                    {
                        current_custom_text[i] = message[i];
                    }
                    current_custom_text_length = message_length;

                    message_length = 0;
                    c = 0;
                    save_current_to_preset(0xff); // save to last config
                    set_console_color(CONSOLE_DGREEN_COLOR);
                    printf("\nMessage saved succesfully.\n");
                    set_console_color(CONSOLE_RESET_COLOR);
                    while (getchar_timeout_us(0) != PICO_ERROR_TIMEOUT)
                    {
                        /* code */
                    }
                    break;
                }
                else if (message_length < 32 && c > 31 && c < 127) // check if buffer isn't full and handle input characters
                {
                    printf("%c", c);
                    message[message_length] = c;
                    message_length++;
                }
                else
                {
                    // buffer is probably full or character is invalid, so do nothing
                    busy_wait_ms(1);
                }
                c = 0;
            }
        }
    }
    else if (compare_command("txtext", 6))
    {
        if (genmode == 0)
        {
            set_console_color(CONSOLE_DRED_COLOR);
            printf("Transmitting message while in carier mode is not posible.\n");
            set_console_color(CONSOLE_RESET_COLOR);
            gpio_put(ERROR_LED_PIN, 1);
            busy_wait_ms(100);
            gpio_put(ERROR_LED_PIN, 0);
        }
        else
        {
            set_console_color(CONSOLE_DGREEN_COLOR);
            printf("Transmitting message \"");
            set_console_color(CONSOLE_DBLUE_COLOR);
            for (int i = 0; i < current_custom_text_length; i++)
            {
                printf("%c", current_custom_text[i]);
            }
            set_console_color(CONSOLE_DGREEN_COLOR);
            printf("\" you can cancel by ctrl+c.\n");
            set_console_color(CONSOLE_WHITE_COLOR);
            for (int i = 0; i < current_custom_text_length; i++)
            {
                printf("%c", current_custom_text[i]);
                switch (genmode)
                {
                case 0:
                    // TODO
                    break;
                case 1:
                    CW_TX_letter(current_custom_text[i]);
                    break;
                case 2:
                    if (i == 0)
                    {
                        RTTYTXletter(current_custom_text[i], 1);
                        busy_wait_us(current_bit_time * 2);
                    }
                    else
                    {
                        RTTYTXletter(current_custom_text[i], 0);
                        busy_wait_us(current_bit_time * 2);
                    }
                    break;
                case 3:
                    HELL_TX_letter(current_custom_text[i]);
                    break;
                default:
                    break;
                }

                c = getchar_timeout_us(0);
                if (c == 3)
                {
                    set_console_color(CONSOLE_YELLOW_COLOR);
                    printf("\nTransmition canceled.");
                    set_console_color(CONSOLE_RESET_COLOR);
                    while (getchar_timeout_us(0) != PICO_ERROR_TIMEOUT)
                    {
                        /* code */
                    }
                    break;
                }
            }
            printf("\n");
            set_console_color(CONSOLE_RESET_COLOR);
        }
    }
    else if (compare_command("hellspeed ", 10))
    {
        uint16_t speed_to_set = string_to_uint16(10);
        if (speed_to_set >= MIN_HELL_SPEED && speed_to_set <= MAX_HELL_SPEED)
        {
            set_HELL_speed(speed_to_set);
            set_console_color(CONSOLE_DGREEN_COLOR);
            printf("Hell speed set to: %u baud\n", current_HELL_speed);
            printf("Bit time: %u us\n", current_HELL_bittime_us);
            set_console_color(CONSOLE_RESET_COLOR);
            switch (genmode)
            {
            case 0:
                // TODO
                break;
            case 1:
                refresh_CW_config();
                break;
            case 2:
                refresh_RTTY_config();
                break;
            case 3:
                refresh_HELL_config();
                break;

            default:
                break;
            }
            save_current_to_preset(0xff); // save to last config
        }
        else
        {
            set_console_color(CONSOLE_DRED_COLOR);
            printf("Hell speed invalid, the format is \"hellspeed x\". min: %d max: %d\n", MIN_HELL_SPEED, MAX_HELL_SPEED);
            set_console_color(CONSOLE_RESET_COLOR);
        }
    }
    else if (compare_command("manhelltx", 9))
    {
        if (genmode == 3)
        {
            int8_t c = ' ';
            HELL_TX_letter(c);
            gpio_put(RUNNING_LED_PIN, 1);
            while (c != 3)
            {
                c = getchar_timeout_us(0);
                if (c != PICO_ERROR_TIMEOUT)
                {

                    if (c == 10 || c == 13)
                    {
                        printf("\n");
                    }
                    else
                    {
                        printf("%c", c);
                        HELL_TX_letter(c);
                    }
                }
                else
                {
                    busy_wait_ms(1);
                }
            }
            gpio_put(RUNNING_LED_PIN, 0);
            printf("\n", c);
        }
        else
        {
            set_console_color(CONSOLE_DRED_COLOR);
            printf("ERROR: incorect mode selected.\n");
            set_console_color(CONSOLE_RESET_COLOR);
        }
    }
    else if (compare_command("txbeacon", 8))
    {
        if (genmode == 0)
        {
            set_console_color(CONSOLE_DRED_COLOR);
            printf("Transmitting message while in carier mode is not posible.\n");
            set_console_color(CONSOLE_RESET_COLOR);
            gpio_put(ERROR_LED_PIN, 1);
            busy_wait_ms(100);
            gpio_put(ERROR_LED_PIN, 0);
        }
        else
        {
            uint8_t beacon_interval = string_to_uint16(8);
            beaconen = 1;
            set_console_color(CONSOLE_DGREEN_COLOR);
            printf("Beaconing message \"");
            set_console_color(CONSOLE_DBLUE_COLOR);
            for (int i = 0; i < current_custom_text_length; i++)
            {
                printf("%c", current_custom_text[i]);
            }
            set_console_color(CONSOLE_DGREEN_COLOR);
            printf("\" in ");
            set_console_color(CONSOLE_DBLUE_COLOR);
            printf("%ds", beacon_interval);
            set_console_color(CONSOLE_DGREEN_COLOR);
            printf(" intervals you can cancel by ctrl+c.\n");
            set_console_color(CONSOLE_WHITE_COLOR);
            while (beaconen)
            {
                for (int i = 0; i < current_custom_text_length; i++)
                {
                    printf("%c", current_custom_text[i]);
                    switch (genmode)
                    {
                    case 0:
                        // TODO
                        break;
                    case 1:
                        CW_TX_letter(current_custom_text[i]);
                        break;
                    case 2:
                        if (i == 0)
                        {
                            RTTYTXletter(current_custom_text[i], 1);
                            busy_wait_us(current_bit_time * 2);
                        }
                        else
                        {
                            RTTYTXletter(current_custom_text[i], 0);
                            busy_wait_us(current_bit_time * 2);
                        }
                        break;
                    case 3:
                        HELL_TX_letter(current_custom_text[i]);
                        break;
                    default:
                        break;
                    }

                    c = getchar_timeout_us(0);
                    if (c == 3)
                    {
                        set_console_color(CONSOLE_YELLOW_COLOR);
                        printf("\nBeacon canceled.");
                        set_console_color(CONSOLE_RESET_COLOR);
                        while (getchar_timeout_us(0) != PICO_ERROR_TIMEOUT)
                        {
                            /* code */
                        }
                        beaconen = 0;
                        break;
                    }
                }
                printf("\n");

                if (genmode == RTTY_MODE)
                {
                    RTTYTXletter(10, 0);
                    busy_wait_us(current_char_delay * 5);
                    RTTYTXletter(13, 0);
                }

                for (uint32_t i = 0; i < beacon_interval * 2; i++)
                {
                    c = getchar_timeout_us(0);
                    if (c == 3)
                    {
                        set_console_color(CONSOLE_YELLOW_COLOR);
                        printf("\nBeacon canceled.");
                        set_console_color(CONSOLE_RESET_COLOR);
                        while (getchar_timeout_us(0) != PICO_ERROR_TIMEOUT)
                        {
                            /* code */
                        }
                        beaconen = 0;
                        break;
                    }
                    busy_wait_ms(500);
                }
            }
            printf("\n");
            set_console_color(CONSOLE_RESET_COLOR);
        }
    }
    else
    {
        set_console_color(CONSOLE_DRED_COLOR);
        printf("Command unknown, type \"help\" for help\n");
        set_console_color(CONSOLE_RESET_COLOR);
    }
}

// if command doesn't match it returns 0, otherwise it returns number of valid characters
uint8_t compare_command(const char *command, uint8_t size)
{
    uint8_t ret = 0;
    for (uint8_t i = 0; i < size; i++)
    {
        if (console_input_buf[i] != command[i])
        {
            return 0;
        }
        else
        {
            ret++;
        }
    }
    return ret;
}

uint8_t valbuf[16];
// any char isn't a number it returns 0, otherwise it returns number of valid characters
uint8_t is_number(uint8_t start, uint8_t size)
{
    uint8_t ret = 0;
    for (uint8_t i = 0; i < size; i++)
    {
        if (console_input_buf[start + i] >= 48 && console_input_buf[start + i] <= 57)
        {
            ret++;
        }
        else
        {
            return 0;
        }
    }

    return ret;
}

// converts ascii char to number
int8_t char_to_number(uint8_t x)
{
    uint8_t ret = 0;
    if (x < 48 || x > 57)
    {
        ret = 0;
    }
    else
    {
        ret = x - 48;
    }

    return ret;
}

// converts ascii "string" to number
uint64_t string_to_uint64(uint8_t start)
{
    uint64_t ret = 0;
    ret = abs(atol(&console_input_buf[start]));

    return ret;
}
// converts ascii "string" to number
uint8_t string_to_uint8(uint8_t start)
{
    uint8_t ret = 0;
    ret = abs(atoi(&console_input_buf[start]));

    return ret;
}
// converts ascii "string" to number
uint16_t string_to_uint16(uint8_t start)
{
    uint16_t ret = 0;
    ret = abs(atoi(&console_input_buf[start]));
    return ret;
}

void set_console_color(uint8_t color)
{
    switch (color)
    {
        case CONSOLE_RESET_COLOR:
        printf("\033[0m");
        break;
        case CONSOLE_DGRAY_COLOR:
        printf("\033[1;30m");
        break;
        case CONSOLE_DRED_COLOR:
        printf("\033[1;31m");
        break;
        case CONSOLE_DGREEN_COLOR:
        printf("\033[1;32m");
        break;
        case CONSOLE_YELLOW_COLOR:
        printf("\033[1;33m");
        break;
        case CONSOLE_DBLUE_COLOR:
        printf("\033[1;34m");
        break;
        case CONSOLE_DMAGENTA_COLOR:
        printf("\033[1;35m");
        break;
        case CONSOLE_DCYAN_COLOR:
        printf("\033[1;36m");
        break;
        case CONSOLE_WHITE_COLOR:
        printf("\033[1;37m");
        break;

    default:
        printf("\033[0m");
        break;
    }
}