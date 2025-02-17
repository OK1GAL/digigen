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
    write_eeprom_single_uint8(address,(data&0xff));
    write_eeprom_single_uint8(address+1,((data>>8)&0xff));
}
void write_eeprom_single_uint32(uint16_t address, uint32_t data)
{
    write_eeprom_single_uint8(address,(data&0xff));
    write_eeprom_single_uint8(address+1,((data>>8)&0xff));
    write_eeprom_single_uint8(address+2,((data>>16)&0xff));
    write_eeprom_single_uint8(address+3,((data>>24)&0xff));
}
void write_eeprom_single_uint64(uint16_t address, uint32_t data)
{
    write_eeprom_single_uint8(address,(data&0xff));
    write_eeprom_single_uint8(address+1,((data>>8)&0xff));
    write_eeprom_single_uint8(address+2,((data>>16)&0xff));
    write_eeprom_single_uint8(address+3,((data>>24)&0xff));
    write_eeprom_single_uint8(address+4,((data>>32)&0xff));
    write_eeprom_single_uint8(address+5,((data>>40)&0xff));
    write_eeprom_single_uint8(address+6,((data>>48)&0xff));
    write_eeprom_single_uint8(address+7,((data>>56)&0xff));
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
    uint64_t data = read_eeprom_single_uint8(start_address+7);
    data = data << 8;
    data = data | read_eeprom_single_uint8(start_address+6);
    data = data << 8;
    data = data | read_eeprom_single_uint8(start_address+5);
    data = data << 8;
    data = data | read_eeprom_single_uint8(start_address+4);
    data = data << 8;
    data = data | read_eeprom_single_uint8(start_address+3);
    data = data << 8;
    data = data | read_eeprom_single_uint8(start_address+2);
    data = data << 8;
    data = data | read_eeprom_single_uint8(start_address+1);
    data = data << 8;
    data = data | read_eeprom_single_uint8(start_address);
    
    return data;
}


#define MODE_MEM_OFFSET 0
#define CFREQ_MEM_OFFSET 1
#define SFREQ_MEM_OFFSET 9
#define BAUDRATE_MEM_OFFSET 17
#define BIT_TIME_MEM_OFFSET 19
#define CHAR_DELAY_MEM_OFFSET 21
#define CW_SPEED_MEM_OFFSET 23
#define CW_BITTIME_MEM_OFFSET 24 //+8 = 32


void save_current_to_preset(uint8_t preset)
{
    uint16_t start_address = 0x40 + preset*0x40;
    write_eeprom_single_uint8(start_address+MODE_MEM_OFFSET,genmode);
    write_eeprom_single_uint64(start_address+CFREQ_MEM_OFFSET,current_center_freq);
    write_eeprom_single_uint64(start_address+SFREQ_MEM_OFFSET,current_frequency_shift);
    write_eeprom_single_uint16(start_address+BAUDRATE_MEM_OFFSET,current_baudrate);
    write_eeprom_single_uint16(start_address+BIT_TIME_MEM_OFFSET,current_bit_time);
    write_eeprom_single_uint16(start_address+CHAR_DELAY_MEM_OFFSET,current_char_delay);
    write_eeprom_single_uint8(start_address+CW_SPEED_MEM_OFFSET,current_CW_speed);
    write_eeprom_single_uint64(start_address+CW_BITTIME_MEM_OFFSET,current_CW_bittime_us);
}

uint8_t current_preset = 7;
uint8_t last_prog_btn_state = 1;
void handle_preset_switching()
{
    gpio_put(PROG1_LED_PIN,current_preset&1);
    gpio_put(PROG2_LED_PIN,(current_preset>>1)&1);
    gpio_put(PROG3_LED_PIN,(current_preset>>2)&1);
    if (!gpio_get(PROGRAM_BTN_PIN) && last_prog_btn_state == 1)
    {
        busy_wait_ms(50);
        current_preset++;
        if (current_preset>7)
        {
            current_preset = 0;
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