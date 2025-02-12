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
uint8_t string_to_uint8(uint8_t start, uint8_t size);
uint64_t string_to_uint64(uint8_t start, uint8_t size);
uint16_t string_to_uint16(uint8_t start, uint8_t size);

void console_init()
{
    while (!stdio_usb_connected())
    {
        busy_wait_ms(1);
    }

    printf("\n\n\n\nWelcome to digigen!\n");
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
        printf("\n");
    }
    else if (compare_command("config", 6))
    {
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
    }
    else if (compare_command("baud ", 5))
    {
        if (is_number(5, 3))
        {
            uint16_t baud_to_set = string_to_uint16(5, 3);
            if (baud_to_set >= MIN_BAUDRATE && baud_to_set <= MAX_BAUDRATE)
            {
                current_baudrate = baud_to_set;
                current_bit_time = ((float)1000000 / (float)current_baudrate) - SYNTH_BIT_SET_TIME_us;
                current_char_delay = (current_bit_time - SYNTH_BIT_SET_TIME_us) * 2;
                printf("Baudrate set to: %u baud\n", current_baudrate);
                printf("Bit time: %u us\n", current_bit_time + SYNTH_BIT_SET_TIME_us);
                refresh_RTTY_config();
            }
            else
            {
                printf("Baudrate invalid\n");
            }
        }
        else
        {
            printf("Baudrate invalid\n");
        }
    }
    else if (compare_command("cfreq ", 6))
    {
        if (is_number(6, 9))
        {
            uint64_t cfreq_to_set = string_to_uint64(6, 9);
            if (cfreq_to_set >= MIN_CFREQ && cfreq_to_set <= MAX_CFREQ)
            {
                current_center_freq = cfreq_to_set;
                printf("Center frequency set to: %lluHz\n", current_center_freq);
                refresh_RTTY_config();
                refresh_CW_config();
            }
            else
            {
                printf("Frequency invalid\n");
            }
        }
        else
        {
            printf("Frequency invalid\n");
        }
    }
    else if (compare_command("sfreq ", 6))
    {
        if (is_number(6, 9))
        {
            uint64_t sfreq_to_set = string_to_uint64(6, 9);
            if (sfreq_to_set >= MIN_SFREQ && sfreq_to_set <= MAX_SFREQ)
            {
                current_frequency_shift = sfreq_to_set;
                printf("Center frequency set to: %lluHz\n", current_frequency_shift);
                refresh_RTTY_config();
            }
            else
            {
                printf("Frequency invalid\n");
            }
        }
        else
        {
            printf("Frequency invalid\n");
        }
    }
    else if (compare_command("cwspeed ", 8))
    {
        if (is_number(8, 2))
        {
            uint64_t cwspeed = string_to_uint64(8, 2);
            if (cwspeed > 0 && cwspeed < 100)
            {
                current_CW_speed = cwspeed;
                set_CW_WPM(current_CW_speed);
                printf("CW speed set to: %d\n", current_CW_speed);
            }
            else
            {
                printf("Invalid speed\n");
            }
        }
        else
        {
            printf("Command invalid\n");
        }
    }
    else if (compare_command("txen ", 5))
    {
        if (is_number(5, 1))
        {
            uint64_t txen = string_to_uint64(5, 1);
            if (txen == 0)
            {
                tx_enable(0);
                printf("TX disabled\n");
            }
            else
            {
                tx_enable(1);
                printf("TX enabled\n");
            }
        }
        else
        {
            printf("Command invalid\n");
        }
    }
    else if (compare_command("manrttytx", 9))
    {
        if (genmode == 2)
        {
            int8_t c = ' ';
            RTTYTXletter(c, 1);
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
            printf("\n", c);
        }
        else
        {
            printf("ERROR: incorect mode selected.\n");
        }
    }
    else if (compare_command("txmode ", 7))
    {
        if (is_number(7, 1))
        {
            uint64_t mode = string_to_uint64(7, 1);
            if (mode >= 0 && mode <= GENMODE_MAX)
            {
                genmode = mode;
                printf("Mode set to %d\n", genmode);
            }
            else
            {
                printf("ERROR: invalid mode\n");
            }
        }
        else
        {
            printf("Command invalid\n");
        }
    }
    else if (compare_command("mancwtx", 7))
    {
        if (genmode == 1)
        {
            int8_t c = ' ';
            CW_TX_letter(c);
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
            printf("\n", c);
        }
        else
        {
            printf("ERROR: incorect mode selected.\n");
        }
    }
    else
    {
        printf("Command unknown, type \"help\" for help\n");
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
uint64_t string_to_uint64(uint8_t start, uint8_t size)
{
    uint64_t ret = 0;
    for (uint8_t i = 0; i < size; i++)
    {
        ret = ret + (char_to_number(console_input_buf[start + i]) * pow(10, size - i - 1));
    }

    return ret;
}
// converts ascii "string" to number
uint8_t string_to_uint8(uint8_t start, uint8_t size)
{
    uint8_t ret = 0;
    for (uint8_t i = 0; i < size; i++)
    {
        ret = ret + (char_to_number(console_input_buf[start + i]) * pow(10, size - i));
    }

    return ret;
}
// converts ascii "string" to number
uint16_t string_to_uint16(uint8_t start, uint8_t size)
{
    uint16_t ret = 0;
    for (uint8_t i = 0; i < size; i++)
    {
        ret = ret + (char_to_number(console_input_buf[start + i]) * pow(10, size - i - 1));
    }

    return ret;
}
