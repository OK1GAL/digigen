#include "header.h"

// For now only CLK0 is usable for FSK

uint64_t pll_freq_mark = 69;
uint64_t pll_freq_space;
uint64_t pll_freq;

struct Si5351RegSet ms_reg_mark;
struct Si5351RegSet ms_reg_space;

uint8_t r_div_mark = 0;
uint8_t r_div_space = 0;

// max 100MHz
void recalculate_fsk_params(uint64_t center_freq, uint64_t frequency_deviation)
{
    uint64_t current_mark_frequency = center_freq + (frequency_deviation);
    uint64_t current_space_frequency = center_freq - (frequency_deviation);
    pll_freq_mark = SI5351_PLL_FIXED;
    pll_freq_space = SI5351_PLL_FIXED;

    // Set PLL
    set_pll(pll_freq_mark, SI5351_PLLA);

    r_div_mark = select_r_div(&current_mark_frequency);
    r_div_space = select_r_div(&current_space_frequency);

    multisynth_calc(current_mark_frequency, pll_freq_mark, &ms_reg_mark);
    multisynth_calc(current_space_frequency, pll_freq_space, &ms_reg_space);

    set_ms(0, ms_reg_mark, 0, r_div_mark, 0);
    pll_reset(SI5351_PLLA);
    set_FSK_symbol(1);
}

void set_FSK_symbol(uint8_t symbol)
{
    if (symbol == 0)
    {
        // set_pll(pll_freq_space, SI5351_PLLA);
        set_ms(0, ms_reg_space, 0, r_div_space, 0);
        gpio_put(SPACE_LED_PIN, 1);
        gpio_put(MARK_LED_PIN, 0);
        gpio_put(TTY_TX_PIN,0);
    }
    else
    {
        // set_pll(pll_freq_mark, SI5351_PLLA);
        set_ms(0, ms_reg_mark, 0, r_div_mark, 0);
        gpio_put(SPACE_LED_PIN, 0);
        gpio_put(MARK_LED_PIN, 1);
        gpio_put(TTY_TX_PIN,1);
    }
    // pll_reset(SI5351_PLLA);
}

void tx_enable(uint8_t enable)
{
    if (enable == 0)
    {
        output_enable(SI5351_CLK0, 0);
    }
    else
    {
        output_enable(SI5351_CLK0, 1);
    }
}

uint8_t set_TX_mode(uint8_t mode)
{
    if (mode <= GENMODE_MAX)
    {
        genmode = mode;
        switch (genmode)
        {
        case 0:
            // constant carier
            break;
        case 1:
            set_CW_mode();
            break;
        case 2:
            // RTTY
            break;
        default:
            return 0;
            break;
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t set_tx_drive_strenght(uint8_t strenght)
{
    if (strenght > 3)
    {
        return 0;
    }
    else
    {
        current_drive_strenght = strenght;
        drive_strength(default_CLK, strenght);
        return 1;
    }
}
