#include "header.h"

// speed explanation
// speed is in WPM
// reference word PARIS
// .--. .- .-. .. ...
// 50 time units
// ms/bit = 1/((WPM/60)*50)

// ascii offset 32
//  : six bit space
// !: -.-.--
// ": null
// #: null
// $: null
// %: null
// &: null
// ': null
// (: null
// ): null
// *: null
// +: .-.-.
// ,: --..--
// -: -....-
// .: .-.-.-
// /: -..-.
// 0: -----     27bits
// 1: .----
// 2: ..---
// 3: ...--
// 4: ....-
// 5: .....
// 6: -....
// 7: --...
// 8: ---..
// 9: ----.
// :: ---...
// ;: -.-.-.
// <: null
// =: -...-
// >: null
// ?: ..--..    25bits
// @: null
// A: .-
// B: -...
// C: -.-.
// D: -..
// E: .
// F: ..-.
// G: --.
// H: ....
// I: ..
// J: .---
// K: -.-
// L: .-..
// M: --
// N: -.
// O: ---
// P: .--.
// Q: --.-
// R: .-.
// S: ...
// T: -
// U: ..-
// V: ...-
// W: .--
// X: -..-
// Y: -.--
// z: --..

#define CW_letters_total 59
uint32_t CW_letters[] = {
    0b000000,                        // <-- if this bothers you, fuck you (space)
    0b11100010001110001000111000111, //!
    0b0,
    0b0,
    0b0,
    0b0,
    0b0,
    0b0,
    0b0,
    0b0,
    0b0,
    0b100011100010001110001,          //+
    0b11100011100010001000111000111,  //,
    0b1110001000100010001000111,      //-
    0b100011100010001110001000111,    //.
    0b111000100010001110001,          ///
    0b111000111000111000111000111,    // 0
    0b1000111000111000111000111,      // 1
    0b10001000111000111000111,        // 2
    0b100010001000111000111,          // 3
    0b1000100010001000111,            // 4
    0b10001000100010001,              // 5
    0b1110001000100010001,            // 6
    0b111000111000100010001,          // 7
    0b11100011100011100010001,        // 8
    0b1110001110001110001110001,      // 9
    0b111000111000111000100010001000, //:
    0b111000100011100010001110001,    //;
    0b0,
    0b111000100010001000111, //=
    0b0,
    0b1000100011100011100010001, //?
    0b0,
    0b1000111,             // A
    0b111000100010001,     // B
    0b11100010001110001,   // C
    0b11100010001,         // D
    0b1,                   // E
    0b100010001110001,     // F
    0b1110001110001,       // G
    0b1000100010001,       // H
    0b10001,               // I
    0b1000111000111000111, // J
    0b1110001000111,       // K
    0b100011100010001,     // L
    0b111000111,           // M
    0b1110001,             // N
    0b111000111000111,     // O
    0b10001110001110001,   // P
    0b1110001110001000111, // Q
    0b10001110001,         // R
    0b100010001,           // S
    0b111,                 // T
    0b10001000111,         // U
    0b100010001000111,     // V
    0b1000111000111,       // W
    0b11100010001000111,   // X
    0b1110001000111000111, // Y
    0b11100011100010001    // Z
};
// all values have to be bit shifted 6 bits
// to left to make character space of 6 time units

uint32_t CW_letters_len[] = {
    6,  // space
    29, //!
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    21, //+
    29, //,
    25, //-
    27, //.
    21, ///
    27, // 0
    25, // 1
    23, // 2
    21, // 3
    19, // 4
    17, // 5
    19, // 6
    21, // 7
    23, // 8
    25, // 9
    30, //:
    27, //;
    0,
    21, //=
    0,
    25, //?
    0,
    7,  // A
    15, // B
    17, // C
    11, // D
    1,  // E
    15, // F
    13, // G
    13, // H
    5,  // I
    19, // J
    13, // K
    15, // L
    9,  // M
    7,  // N
    15, // O
    17, // P
    19, // Q
    11, // R
    9,  // S
    3,  // T
    11, // U
    15, // V
    13, // W
    17, // X
    19, // Y
    17  // Z
};

void set_CW_mode()
{
    set_freq(current_center_freq, SI5351_CLK0);
    tx_enable(0);
}

void refresh_CW_config()
{
    set_freq(current_center_freq, SI5351_CLK0);
    set_CW_WPM(current_CW_speed);
}

void set_CW_WPM(uint8_t WPM)
{
    current_CW_speed = WPM;
    current_CW_bittime_us = (1 / (float)(((float)current_CW_speed / 60) * 50)) * 1000000;
}

void CW_TX_letter(uint8_t charin)
{
    if (charin >= 97 && charin <= 122)
    {
        charin = charin - 32;
    }
    charin = charin - 32;
    if (charin > CW_letters_total)
    {
        charin = 31; //? if out of range
    }

    uint64_t charin64 = CW_letters[charin] << 6;

    for (int i = CW_letters_len[charin] + 6; i >= 0; i--)
    {
        tx_enable((charin64 >> i) & 0b1);
        if ((charin64 >> i) & 0b1)
        {
            busy_wait_us(current_CW_bittime_us >>1);
        }

        busy_wait_us(current_CW_bittime_us);
    }
    tx_enable(0);
}
