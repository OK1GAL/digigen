#include "header.h"

char ascii2baudot[130] = {0B1000000, 0B1000000, 0B1000000, 0B1000000, 0B0110100, 0B1000000, 0B1000000, 0B0101011, 0B1000000, 0B1000100,
                          0B1000010, 0B1000000, 0B1000010, 0B1001000, 0B1000000, 0B1000000, 0B1000000, 0B1000000, 0B1000000, 0B1000000,
                          0B1000000, 0B1000000, 0B1000000, 0B1000000, 0B1000000, 0B1000000, 0B1000000, 0B1000000, 0B1000000, 0B1000000,
                          0B1000000, 0B1000000, 0B1000100, 0B0101101, 0B0100101, 0B1000000, 0B0101001, 0B1000000, 0B0111010, 0B0100101,
                          0B0101111, 0B0110010, 0B1000000, 0B1000000, 0B0101100, 0B0100011, 0B0111100, 0B0111101, 0B0110110, 0B0110111,
                          0B0110011, 0B0100001, 0B0101010, 0B0110000, 0B0110101, 0B0100111, 0B0100110, 0B0111000, 0B0101110, 0B0111110,
                          0B1000000, 0B1000000, 0B1000000, 0B0111001, 0B1000000, 0B0000011, 0B0011001, 0B0001110, 0B0001001, 0B0000001,
                          0B0001101, 0B0011010, 0B0010100, 0B0000110, 0B0001011, 0B0001111, 0B0010010, 0B0011100, 0B0001100, 0B0011000,
                          0B0010110, 0B0010111, 0B0001010, 0B0000101, 0B0010000, 0B0000111, 0B0011110, 0B0010011, 0B0011101, 0B0010101,
                          0B0010001, 0B1000000, 0B1000000, 0B1000000, 0B1000000, 0B1000000, 0B1000000, 0B0000011, 0B0011001, 0B0001110,
                          0B0001001, 0B0000001, 0B0001101, 0B0011010, 0B0010100, 0B0000110, 0B0001011, 0B0001111, 0B0010010, 0B0011100,
                          0B0001100, 0B0011000, 0B0010110, 0B0010111, 0B0001010, 0B0000101, 0B0010000, 0B0000111, 0B0011110, 0B0010011,
                          0B0011101, 0B0010101, 0B0010001, 0B1000000, 0B1000000, 0B1000000, 0B1000000, 0B1011111, 0B1011111, 0B1011011};
 


void RTTYTXletterRAW_blocking(uint8_t letter)
{
    set_FSK_symbol(0);
    for (int i = 0; i < 5; i++)
    {
        busy_wait_us(current_bit_time);
        set_FSK_symbol(((letter >> i) & 0x01));
    }
    busy_wait_us(current_bit_time);
    set_FSK_symbol(1);
}

int lastchar = 0;
void RTTYTXletter(uint8_t letter, uint8_t forcechange)
{
    int chartype = ((ascii2baudot[letter] >> 5) & 0x01); // check if current char is letter or figure
    if ((chartype != lastchar) || forcechange)
    {
        set_FSK_symbol(0);
        for (int i = 0; i < 5; i++)
        {
            busy_wait_us(current_bit_time);
            set_FSK_symbol((ascii2baudot[128 + chartype] >> i) & 0x01);
        }
        busy_wait_us(current_bit_time);
        set_FSK_symbol(1);

        lastchar = chartype;
        busy_wait_us(current_char_delay);
    }

    set_FSK_symbol(0);
    for (int i = 0; i < 5; i++)
    {
        busy_wait_us(current_bit_time);
        set_FSK_symbol((ascii2baudot[letter] >> i) & 0x01);
    }
    busy_wait_us(current_bit_time);
    set_FSK_symbol(1);
}

void set_RTTY_mode()
{
    recalculate_fsk_params(current_center_freq, current_frequency_shift);
    set_FSK_symbol(1);
    tx_enable(0);
}

void refresh_RTTY_config()
{
    recalculate_fsk_params(current_center_freq, current_frequency_shift);
    set_FSK_symbol(1);
}