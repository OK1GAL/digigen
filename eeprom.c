#include "header.h"

void write_eeprom_single_uint8(uint16_t address, uint8_t data)
{
    uint8_t data_to_send[3] = {0, 0, 0};
    data_to_send[0] = address >> 8;
    data_to_send[1] = address & 0xFF;
    data_to_send[2] = data;
    i2c_write_blocking(I2C_PORT, EEPROM_ADDR, &data_to_send, 3, 0);
    busy_wait_ms(5);
}

void write_eeprom_single_uint16(uint16_t address, uint16_t data)
{
    write_eeprom_single_uint8(address, (data & 0xff));
    write_eeprom_single_uint8(address + 1, ((data >> 8) & 0xff));
}
void write_eeprom_single_uint32(uint16_t address, uint32_t data)
{
    write_eeprom_single_uint8(address, (data & 0xff));
    write_eeprom_single_uint8(address + 1, ((data >> 8) & 0xff));
    write_eeprom_single_uint8(address + 2, ((data >> 16) & 0xff));
    write_eeprom_single_uint8(address + 3, ((data >> 24) & 0xff));
}
void write_eeprom_single_uint64(uint16_t address, uint32_t data)
{
    write_eeprom_single_uint8(address, (data & 0xff));
    write_eeprom_single_uint8(address + 1, ((data >> 8) & 0xff));
    write_eeprom_single_uint8(address + 2, ((data >> 16) & 0xff));
    write_eeprom_single_uint8(address + 3, ((data >> 24) & 0xff));
    write_eeprom_single_uint8(address + 4, ((data >> 32) & 0xff));
    write_eeprom_single_uint8(address + 5, ((data >> 40) & 0xff));
    write_eeprom_single_uint8(address + 6, ((data >> 48) & 0xff));
    write_eeprom_single_uint8(address + 7, ((data >> 56) & 0xff));
}

uint8_t read_eeprom_single_uint8(uint16_t address)
{
    uint8_t data_to_send[2] = {0, 0};
    uint8_t rxed_data = 0;
    data_to_send[0] = address >> 8;
    data_to_send[1] = address & 0xFF;
    i2c_write_blocking(I2C_PORT, EEPROM_ADDR, &data_to_send, 2, 1);
    i2c_read_blocking(I2C_PORT, EEPROM_ADDR, &rxed_data, 1, 0);

    return rxed_data;
}

uint16_t read_eeprom_single_uint16(uint16_t start_address)
{
    uint16_t data = read_eeprom_single_uint8(start_address + 1);
    data = data << 8;
    data = data + read_eeprom_single_uint8(start_address);
    return data;
}

uint32_t read_eeprom_single_uint32(uint16_t start_address)
{
    uint32_t data = read_eeprom_single_uint8(start_address + 3);
    data = data << 8;
    data = data | read_eeprom_single_uint8(start_address + 2);
    data = data << 8;
    data = data | read_eeprom_single_uint8(start_address + 1);
    data = data << 8;
    data = data | read_eeprom_single_uint8(start_address);
    return data;
}

uint64_t read_eeprom_single_uint64(uint16_t start_address)
{
    uint64_t data = read_eeprom_single_uint8(start_address + 7);
    data = data << 8;
    data = data | read_eeprom_single_uint8(start_address + 6);
    data = data << 8;
    data = data | read_eeprom_single_uint8(start_address + 5);
    data = data << 8;
    data = data | read_eeprom_single_uint8(start_address + 4);
    data = data << 8;
    data = data | read_eeprom_single_uint8(start_address + 3);
    data = data << 8;
    data = data | read_eeprom_single_uint8(start_address + 2);
    data = data << 8;
    data = data | read_eeprom_single_uint8(start_address + 1);
    data = data << 8;
    data = data | read_eeprom_single_uint8(start_address);

    return data;
}

#define MODE_MEM_OFFSET 0
#define CFREQ_MEM_OFFSET 1
#define SFREQ_MEM_OFFSET 9
#define BAUDRATE_MEM_OFFSET 17
#define CW_SPEED_MEM_OFFSET 19
#define DRIVE_STRENGHT_MEM_OFFSET 20 //+1 = 21
#define TEXT_LENGTH_MEM_OFFSET 31
#define TEXT_MEM_OFFSET 32

uint8_t save_current_to_preset(uint8_t preset)
{
    if (preset >= 0 && preset <= 7)
    {
        uint16_t start_address = 0x40 + preset * 0x40;
        write_eeprom_single_uint8(start_address + MODE_MEM_OFFSET, genmode);
        write_eeprom_single_uint64(start_address + CFREQ_MEM_OFFSET, current_center_freq);
        write_eeprom_single_uint64(start_address + SFREQ_MEM_OFFSET, current_frequency_shift);
        write_eeprom_single_uint16(start_address + BAUDRATE_MEM_OFFSET, current_baudrate);
        write_eeprom_single_uint8(start_address + CW_SPEED_MEM_OFFSET, current_CW_speed);
        write_eeprom_single_uint8(start_address + DRIVE_STRENGHT_MEM_OFFSET, current_drive_strenght);
        write_eeprom_single_uint8(start_address + TEXT_LENGTH_MEM_OFFSET, current_custom_text_length);
        for (int i = 0; i < current_custom_text_length; i++)
        {
            write_eeprom_single_uint8(start_address + TEXT_MEM_OFFSET + i, current_custom_text[i]);
        }
        return 1;
    }
    else if (preset == 0xff)
    {
        uint16_t start_address = 0x00;
        write_eeprom_single_uint8(start_address + MODE_MEM_OFFSET, genmode);
        write_eeprom_single_uint64(start_address + CFREQ_MEM_OFFSET, current_center_freq);
        write_eeprom_single_uint64(start_address + SFREQ_MEM_OFFSET, current_frequency_shift);
        write_eeprom_single_uint16(start_address + BAUDRATE_MEM_OFFSET, current_baudrate);
        write_eeprom_single_uint8(start_address + CW_SPEED_MEM_OFFSET, current_CW_speed);
        write_eeprom_single_uint8(start_address + DRIVE_STRENGHT_MEM_OFFSET, current_drive_strenght);
        write_eeprom_single_uint8(start_address + TEXT_LENGTH_MEM_OFFSET, current_custom_text_length);
        for (int i = 0; i < current_custom_text_length; i++)
        {
            write_eeprom_single_uint8(start_address + TEXT_MEM_OFFSET + i, current_custom_text[i]);
        }
        return 1;
    }
    else
    {
        printf("ERROR: preset value out of range. (saving preset)\n");
        gpio_put(ERROR_LED_PIN, 1);
        busy_wait_ms(100);
        gpio_put(ERROR_LED_PIN, 0);
        return 0;
    }
}

uint8_t current_btn_preset = 7;
uint8_t last_prog_btn_state = 1;
void handle_preset_switching()
{
    gpio_put(PROG1_LED_PIN, current_btn_preset & 1);
    gpio_put(PROG2_LED_PIN, (current_btn_preset >> 1) & 1);
    gpio_put(PROG3_LED_PIN, (current_btn_preset >> 2) & 1);
    if (!gpio_get(PROGRAM_BTN_PIN) && last_prog_btn_state == 1)
    {
        busy_wait_ms(100);
        current_btn_preset++;
        if (current_btn_preset > 7)
        {
            current_btn_preset = 0;
        }

        last_prog_btn_state = 0;
    }
    else
    {
        if (gpio_get(PROGRAM_BTN_PIN))
        {
            last_prog_btn_state = 1;
        }
    }
}

uint8_t last_run_eeprom_btn_state = 1;
void handle_run_eeprom_btn()
{
    int8_t c = 0;
    if (!gpio_get(RUN_FROM_EEPROM_BTN_PIN) && last_run_eeprom_btn_state == 1)
    {
        gpio_put(PROG1_LED_PIN, 1);
        gpio_put(PROG2_LED_PIN, 1);
        gpio_put(PROG3_LED_PIN, 1);
        busy_wait_ms(100);
        gpio_put(PROG1_LED_PIN, 0);
        gpio_put(PROG2_LED_PIN, 0);
        gpio_put(PROG3_LED_PIN, 0);
        busy_wait_ms(100);
        gpio_put(PROG1_LED_PIN, current_btn_preset & 1);
        gpio_put(PROG2_LED_PIN, (current_btn_preset >> 1) & 1);
        gpio_put(PROG3_LED_PIN, (current_btn_preset >> 2) & 1);
        busy_wait_ms(50);
        load_preset(current_btn_preset);

        last_run_eeprom_btn_state = 0;

        if (genmode == 0)
        {
            printf("Transmitting message while in carier mode is not posible.\n");
            gpio_put(ERROR_LED_PIN, 1);
            busy_wait_ms(100);
            gpio_put(ERROR_LED_PIN, 0);
        }
        else
        {
            printf("Transmitting message \"");
            for (int i = 0; i < current_custom_text_length; i++)
            {
                printf("%c", current_custom_text[i]);
            }
            printf("\" you can cancel by ctrl+c.\n");

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
                    }
                    else
                    {
                        RTTYTXletter(current_custom_text[i], 0);
                    }
                    break;
                default:
                    break;
                }

                c = getchar_timeout_us(0);
                if (c == 3)
                {
                    printf("\nTransmition canceled.");
                    while (getchar_timeout_us(0) != PICO_ERROR_TIMEOUT)
                    {
                        /* code */
                    }
                    break;
                }
            }
            printf("\n--");
        }
    }
    else
    {
        if (gpio_get(RUN_FROM_EEPROM_BTN_PIN))
        {
            last_run_eeprom_btn_state = 1;
        }
    }
}

uint8_t load_preset(uint8_t preset)
{
    if (preset >= 0 && preset <= 7)
    {
        uint16_t start_address = 0x40 + preset * 0x40;
        genmode = read_eeprom_single_uint8(start_address + MODE_MEM_OFFSET);
        current_center_freq = read_eeprom_single_uint64(start_address + CFREQ_MEM_OFFSET);
        current_frequency_shift = read_eeprom_single_uint64(start_address + SFREQ_MEM_OFFSET);
        current_baudrate = read_eeprom_single_uint16(start_address + BAUDRATE_MEM_OFFSET);
        current_CW_speed = read_eeprom_single_uint8(start_address + CW_SPEED_MEM_OFFSET);
        current_drive_strenght = read_eeprom_single_uint8(start_address + DRIVE_STRENGHT_MEM_OFFSET);
        current_custom_text_length = read_eeprom_single_uint8(start_address + TEXT_LENGTH_MEM_OFFSET);
        for (int i = 0; i < current_custom_text_length; i++)
        {
            current_custom_text[i] = read_eeprom_single_uint8(start_address + TEXT_MEM_OFFSET + i);
        }

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

        default:
            break;
        }
        return 1;
    }
    else if (preset == 0xff)
    {
        uint16_t start_address = 0;
        genmode = read_eeprom_single_uint8(start_address + MODE_MEM_OFFSET);
        current_center_freq = read_eeprom_single_uint64(start_address + CFREQ_MEM_OFFSET);
        current_frequency_shift = read_eeprom_single_uint64(start_address + SFREQ_MEM_OFFSET);
        current_baudrate = read_eeprom_single_uint16(start_address + BAUDRATE_MEM_OFFSET);
        current_CW_speed = read_eeprom_single_uint8(start_address + CW_SPEED_MEM_OFFSET);
        current_drive_strenght = read_eeprom_single_uint8(start_address + DRIVE_STRENGHT_MEM_OFFSET);
        current_custom_text_length = read_eeprom_single_uint8(start_address + TEXT_LENGTH_MEM_OFFSET);
        for (int i = 0; i < current_custom_text_length; i++)
        {
            current_custom_text[i] = read_eeprom_single_uint8(start_address + TEXT_MEM_OFFSET + i);
        }

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

        default:
            break;
        }
        return 1;
    }
    else
    {
        printf("ERROR: preset value out of range. (loading preset)\n");
        gpio_put(ERROR_LED_PIN, 1);
        busy_wait_ms(100);
        gpio_put(ERROR_LED_PIN, 0);
        return 0;
    }
}

uint8_t default_text[32] = "RYRYRYRYRYRYRYRYRYRYRYRYRYRYRYRY";
uint8_t initialize_memory()
{
    for (int i = 0; i <= 8; i++)
    {
        uint16_t start_address = i * 0x40;
        write_eeprom_single_uint8(start_address + MODE_MEM_OFFSET, 0);
        write_eeprom_single_uint64(start_address + CFREQ_MEM_OFFSET, DEFAULT_CENTER_FREQUENCY);
        write_eeprom_single_uint64(start_address + SFREQ_MEM_OFFSET, DEFAULT_FREQUENCY_SHIFT);
        write_eeprom_single_uint16(start_address + BAUDRATE_MEM_OFFSET, DEFAULT_BAUDRATE);
        write_eeprom_single_uint8(start_address + CW_SPEED_MEM_OFFSET, DEFAULT_CW_SPEED);
        write_eeprom_single_uint8(start_address + DRIVE_STRENGHT_MEM_OFFSET, 0);
        write_eeprom_single_uint8(start_address + TEXT_LENGTH_MEM_OFFSET, 32);
        for (int i = 0; i < 32; i++)
        {
            write_eeprom_single_uint8(start_address + TEXT_MEM_OFFSET + i, default_text[i]);
        }
    }
}

uint8_t default_memory()
{
    for (int i = 0; i <= 8; i++)
    {
        uint16_t start_address = i * 0x40;
        write_eeprom_single_uint8(start_address + MODE_MEM_OFFSET, 0);
        write_eeprom_single_uint64(start_address + CFREQ_MEM_OFFSET, DEFAULT_CENTER_FREQUENCY);
        write_eeprom_single_uint64(start_address + SFREQ_MEM_OFFSET, DEFAULT_FREQUENCY_SHIFT);
        write_eeprom_single_uint16(start_address + BAUDRATE_MEM_OFFSET, DEFAULT_BAUDRATE);
        write_eeprom_single_uint8(start_address + CW_SPEED_MEM_OFFSET, DEFAULT_CW_SPEED);
        write_eeprom_single_uint8(start_address + DRIVE_STRENGHT_MEM_OFFSET, 0);
        write_eeprom_single_uint8(start_address + TEXT_LENGTH_MEM_OFFSET, 32);
        for (int i = 0; i < 32; i++)
        {
            write_eeprom_single_uint8(start_address + TEXT_MEM_OFFSET + i, default_text[i]);
        }
    }
}