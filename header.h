#ifndef LIB_INIT
#define LIB_INIT

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h> 
#include <math.h>
#include <inttypes.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"
#include "pico/multicore.h"


#define GENMODE_MAX 3
extern uint8_t genmode;
extern uint8_t current_drive_strenght;

#define default_CLK SI5351_CLK0

//RTTY
#define DEFAULT_CENTER_FREQUENCY 7100000
#define DEFAULT_FREQUENCY_SHIFT 125 // distance from center freq
#define DEFAULT_BAUDRATE 50        // delay between bits
#define SYNTH_SETTING_FREQ_us 1000       // delay between chars
#define DEFAULT_BIT_TIME_us 20000
#define DEFAULT_CHAR_DELAY_us 20000
#define SYNTH_BIT_SET_TIME_us 840

extern uint64_t current_center_freq;
extern uint64_t current_frequency_shift;
extern uint16_t current_baudrate;
extern uint16_t current_bit_time;
extern uint16_t current_char_delay;
extern uint16_t current_HELL_bittime_us;
extern uint8_t current_HELL_speed;

extern uint8_t current_custom_text[32];
extern uint8_t current_custom_text_length;

/*
Memory size per configuration:

mode 1

current_center_freq 8
current_frequency_shift 8
current_baudrate 2
current_bit_time 2
current_char_delay 2

current_CW_speed 1
current_CW_bittime_us 8

sum: 32 bytes

*/

//CW
#define DEFAULT_CW_SPEED 20
#define DEFAULT_CW_BIT_TIME_us 60000 

extern uint8_t current_CW_speed;
extern uint64_t current_CW_bittime_us;

//************GPIO*************//

#define ERROR_LED_PIN 2
#define TTY_TX_PIN 8
#define TTY_RX_PIN 9
#define MODE0_LED_PIN 10
#define MODE1_LED_PIN 11
#define RUNNING_LED_PIN 13
#define SPACE_LED_PIN 15
#define MARK_LED_PIN 14
#define PROG1_LED_PIN 16
#define PROG2_LED_PIN 17
#define PROG3_LED_PIN 18

#define PROGRAM_BTN_PIN 20
#define TEST_BTN_PIN 21
#define RUN_FROM_EEPROM_BTN_PIN 22

//************CONSOLE**********//
#define STDIO_WAIT_TIME_us 5000000
void console_init();
void handle_console_rx();
void print_current_config();

//*********GENERAL TX**********//
void recalculate_fsk_params(uint64_t center_freq, uint64_t frequency_deviation);
void set_FSK_symbol(uint8_t symbol);
void tx_enable(uint8_t enable);
uint8_t set_tx_drive_strenght(uint8_t strenght);

//*************CW**************//
void set_CW_mode();
void refresh_CW_config();
void set_CW_WPM(uint8_t WPM);
void CW_TX_letter(uint8_t charin);

//************RTTY*************//
void RTTYTXletter(uint8_t letter, uint8_t forcechange);
void refresh_RTTY_config();
void set_baudrate(uint16_t baudrate);
void set_RTTY_mode();

//************HELL*************//
void set_HELL_mode();
void refresh_HELL_config();
void set_HELL_speed(uint16_t speed);
void HELL_TX_letter(uint8_t charin);

//***********EEPROM************//
#define I2C_PORT i2c0
#define EEPROM_ADDR 0x50

uint8_t initialize_memory();
uint8_t default_memory();


//***********PRESETS***********//

void handle_preset_switching();
uint8_t load_preset(uint8_t preset);
void handle_run_eeprom_btn();
uint8_t save_current_to_preset(uint8_t preset);


//***********SI5351************//
#define SI5351_I2C_PORT i2c0
#define SI5351_I2C_SCL 5
#define SI5351_I2C_SDA 4

/* Define definitions */

#define SI5351_BUS_BASE_ADDR 0x60
#define SI5351_XTAL_FREQ 25000000
#define SI5351_PLL_FIXED 80000000000ULL
#define SI5351_FREQ_MULT 100ULL
#define SI5351_DEFAULT_CLK 1000000000ULL

#define SI5351_PLL_VCO_MIN 600000000
#define SI5351_PLL_VCO_MAX 900000000
#define SI5351_MULTISYNTH_MIN_FREQ 500000
#define SI5351_MULTISYNTH_DIVBY4_FREQ 150000000
#define SI5351_MULTISYNTH_MAX_FREQ 225000000
#define SI5351_MULTISYNTH_SHARE_MAX 100000000
#define SI5351_MULTISYNTH_SHARE_MIN 1024000
#define SI5351_MULTISYNTH67_MAX_FREQ SI5351_MULTISYNTH_DIVBY4_FREQ
#define SI5351_CLKOUT_MIN_FREQ 4000
#define SI5351_CLKOUT_MAX_FREQ SI5351_MULTISYNTH_MAX_FREQ
#define SI5351_CLKOUT67_MS_MIN SI5351_PLL_VCO_MIN / SI5351_MULTISYNTH67_A_MAX
#define SI5351_CLKOUT67_MIN_FREQ SI5351_CLKOUT67_MS_MIN / 128
#define SI5351_CLKOUT67_MAX_FREQ SI5351_MULTISYNTH67_MAX_FREQ

#define SI5351_PLL_A_MIN 15
#define SI5351_PLL_A_MAX 90
#define SI5351_PLL_B_MAX (SI5351_PLL_C_MAX - 1)
#define SI5351_PLL_C_MAX 1048575
#define SI5351_MULTISYNTH_A_MIN 6
#define SI5351_MULTISYNTH_A_MAX 1800
#define SI5351_MULTISYNTH67_A_MAX 254
#define SI5351_MULTISYNTH_B_MAX (SI5351_MULTISYNTH_C_MAX - 1)
#define SI5351_MULTISYNTH_C_MAX 1048575
#define SI5351_MULTISYNTH_P1_MAX ((1 << 18) - 1)
#define SI5351_MULTISYNTH_P2_MAX ((1 << 20) - 1)
#define SI5351_MULTISYNTH_P3_MAX ((1 << 20) - 1)
#define SI5351_VCXO_PULL_MIN 30
#define SI5351_VCXO_PULL_MAX 240
#define SI5351_VCXO_MARGIN 103

#define SI5351_DEVICE_STATUS 0
#define SI5351_INTERRUPT_STATUS 1
#define SI5351_INTERRUPT_MASK 2
#define SI5351_STATUS_SYS_INIT (1 << 7)
#define SI5351_STATUS_LOL_B (1 << 6)
#define SI5351_STATUS_LOL_A (1 << 5)
#define SI5351_STATUS_LOS (1 << 4)
#define SI5351_OUTPUT_ENABLE_CTRL 3
#define SI5351_OEB_PIN_ENABLE_CTRL 9
#define SI5351_PLL_INPUT_SOURCE 15
#define SI5351_CLKIN_DIV_MASK (3 << 6)
#define SI5351_CLKIN_DIV_1 (0 << 6)
#define SI5351_CLKIN_DIV_2 (1 << 6)
#define SI5351_CLKIN_DIV_4 (2 << 6)
#define SI5351_CLKIN_DIV_8 (3 << 6)
#define SI5351_PLLB_SOURCE (1 << 3)
#define SI5351_PLLA_SOURCE (1 << 2)

#define SI5351_CLK0_CTRL 16
#define SI5351_CLK1_CTRL 17
#define SI5351_CLK2_CTRL 18
#define SI5351_CLK3_CTRL 19
#define SI5351_CLK4_CTRL 20
#define SI5351_CLK5_CTRL 21
#define SI5351_CLK6_CTRL 22
#define SI5351_CLK7_CTRL 23
#define SI5351_CLK_POWERDOWN (1 << 7)
#define SI5351_CLK_INTEGER_MODE (1 << 6)
#define SI5351_CLK_PLL_SELECT (1 << 5)
#define SI5351_CLK_INVERT (1 << 4)
#define SI5351_CLK_INPUT_MASK (3 << 2)
#define SI5351_CLK_INPUT_XTAL (0 << 2)
#define SI5351_CLK_INPUT_CLKIN (1 << 2)
#define SI5351_CLK_INPUT_MULTISYNTH_0_4 (2 << 2)
#define SI5351_CLK_INPUT_MULTISYNTH_N (3 << 2)
#define SI5351_CLK_DRIVE_STRENGTH_MASK (3 << 0)
#define SI5351_CLK_DRIVE_STRENGTH_2MA (0 << 0)
#define SI5351_CLK_DRIVE_STRENGTH_4MA (1 << 0)
#define SI5351_CLK_DRIVE_STRENGTH_6MA (2 << 0)
#define SI5351_CLK_DRIVE_STRENGTH_8MA (3 << 0)

#define SI5351_CLK3_0_DISABLE_STATE 24
#define SI5351_CLK7_4_DISABLE_STATE 25
#define SI5351_CLK_DISABLE_STATE_MASK 3
#define SI5351_CLK_DISABLE_STATE_LOW 0
#define SI5351_CLK_DISABLE_STATE_HIGH 1
#define SI5351_CLK_DISABLE_STATE_FLOAT 2
#define SI5351_CLK_DISABLE_STATE_NEVER 3

#define SI5351_PARAMETERS_LENGTH 8
#define SI5351_PLLA_PARAMETERS 26
#define SI5351_PLLB_PARAMETERS 34
#define SI5351_CLK0_PARAMETERS 42
#define SI5351_CLK1_PARAMETERS 50
#define SI5351_CLK2_PARAMETERS 58
#define SI5351_CLK3_PARAMETERS 66
#define SI5351_CLK4_PARAMETERS 74
#define SI5351_CLK5_PARAMETERS 82
#define SI5351_CLK6_PARAMETERS 90
#define SI5351_CLK7_PARAMETERS 91
#define SI5351_CLK6_7_OUTPUT_DIVIDER 92
#define SI5351_OUTPUT_CLK_DIV_MASK (7 << 4)
#define SI5351_OUTPUT_CLK6_DIV_MASK (7 << 0)
#define SI5351_OUTPUT_CLK_DIV_SHIFT 4
#define SI5351_OUTPUT_CLK_DIV6_SHIFT 0
#define SI5351_OUTPUT_CLK_DIV_1 0
#define SI5351_OUTPUT_CLK_DIV_2 1
#define SI5351_OUTPUT_CLK_DIV_4 2
#define SI5351_OUTPUT_CLK_DIV_8 3
#define SI5351_OUTPUT_CLK_DIV_16 4
#define SI5351_OUTPUT_CLK_DIV_32 5
#define SI5351_OUTPUT_CLK_DIV_64 6
#define SI5351_OUTPUT_CLK_DIV_128 7
#define SI5351_OUTPUT_CLK_DIVBY4 (3 << 2)

#define SI5351_SSC_PARAM0 149
#define SI5351_SSC_PARAM1 150
#define SI5351_SSC_PARAM2 151
#define SI5351_SSC_PARAM3 152
#define SI5351_SSC_PARAM4 153
#define SI5351_SSC_PARAM5 154
#define SI5351_SSC_PARAM6 155
#define SI5351_SSC_PARAM7 156
#define SI5351_SSC_PARAM8 157
#define SI5351_SSC_PARAM9 158
#define SI5351_SSC_PARAM10 159
#define SI5351_SSC_PARAM11 160
#define SI5351_SSC_PARAM12 161

#define SI5351_VXCO_PARAMETERS_LOW 162
#define SI5351_VXCO_PARAMETERS_MID 163
#define SI5351_VXCO_PARAMETERS_HIGH 164

#define SI5351_CLK0_PHASE_OFFSET 165
#define SI5351_CLK1_PHASE_OFFSET 166
#define SI5351_CLK2_PHASE_OFFSET 167
#define SI5351_CLK3_PHASE_OFFSET 168
#define SI5351_CLK4_PHASE_OFFSET 169
#define SI5351_CLK5_PHASE_OFFSET 170

#define SI5351_PLL_RESET 177
#define SI5351_PLL_RESET_B (1 << 7)
#define SI5351_PLL_RESET_A (1 << 5)

#define SI5351_CRYSTAL_LOAD 183
#define SI5351_CRYSTAL_LOAD_MASK (3 << 6)
#define SI5351_CRYSTAL_LOAD_0PF (0 << 6)
#define SI5351_CRYSTAL_LOAD_6PF (1 << 6)
#define SI5351_CRYSTAL_LOAD_8PF (2 << 6)
#define SI5351_CRYSTAL_LOAD_10PF (3 << 6)

#define SI5351_FANOUT_ENABLE 187
#define SI5351_CLKIN_ENABLE (1 << 7)
#define SI5351_XTAL_ENABLE (1 << 6)
#define SI5351_MULTISYNTH_ENABLE (1 << 4)

/* Macro definitions */

// #define RFRAC_DENOM ((1L << 20) - 1)
#define RFRAC_DENOM 1000000ULL

/*
 * Based on former asm-ppc/div64.h and asm-m68knommu/div64.h
 *
 * The semantics of do_div() are:
 *
 * uint32_t do_div(uint64_t *n, uint32_t base)
 * {
 *      uint32_t remainder = *n % base;
 *      *n = *n / base;
 *      return remainder;
 * }
 *
 * NOTE: macro parameter n is evaluated multiple times,
 *       beware of side effects!
 */

#define do_div(n, base) ({            \
	uint64_t __base = (base);         \
	uint64_t __rem;                   \
	__rem = ((uint64_t)(n)) % __base; \
	(n) = ((uint64_t)(n)) / __base;   \
	__rem;                            \
})

/* Enum definitions */

/*
 * enum si5351_variant - SiLabs Si5351 chip variant
 * @SI5351_VARIANT_A: Si5351A (8 output clocks, XTAL input)
 * @SI5351_VARIANT_A3: Si5351A MSOP10 (3 output clocks, XTAL input)
 * @SI5351_VARIANT_B: Si5351B (8 output clocks, XTAL/VXCO input)
 * @SI5351_VARIANT_C: Si5351C (8 output clocks, XTAL/CLKIN input)
 */
/*
enum si5351_variant {
	SI5351_VARIANT_A = 1,
	SI5351_VARIANT_A3 = 2,
	SI5351_VARIANT_B = 3,
	SI5351_VARIANT_C = 4,
};
*/

enum si5351_clock
{
	SI5351_CLK0,
	SI5351_CLK1,
	SI5351_CLK2,
	SI5351_CLK3,
	SI5351_CLK4,
	SI5351_CLK5,
	SI5351_CLK6,
	SI5351_CLK7
};

enum si5351_pll
{
	SI5351_PLLA,
	SI5351_PLLB
};

enum si5351_drive
{
	SI5351_DRIVE_2MA,
	SI5351_DRIVE_4MA,
	SI5351_DRIVE_6MA,
	SI5351_DRIVE_8MA
};

enum si5351_clock_source
{
	SI5351_CLK_SRC_XTAL,
	SI5351_CLK_SRC_CLKIN,
	SI5351_CLK_SRC_MS0,
	SI5351_CLK_SRC_MS
};

enum si5351_clock_disable
{
	SI5351_CLK_DISABLE_LOW,
	SI5351_CLK_DISABLE_HIGH,
	SI5351_CLK_DISABLE_HI_Z,
	SI5351_CLK_DISABLE_NEVER
};

enum si5351_clock_fanout
{
	SI5351_FANOUT_CLKIN,
	SI5351_FANOUT_XO,
	SI5351_FANOUT_MS
};

enum si5351_pll_input
{
	SI5351_PLL_INPUT_XO,
	SI5351_PLL_INPUT_CLKIN
};

/* Struct definitions */

struct Si5351RegSet
{
	uint32_t p1;
	uint32_t p2;
	uint32_t p3;
};

struct Si5351Status
{
	uint8_t SYS_INIT;
	uint8_t LOL_B;
	uint8_t LOL_A;
	uint8_t LOS;
	uint8_t REVID;
};

struct Si5351IntStatus
{
	uint8_t SYS_INIT_STKY;
	uint8_t LOL_B_STKY;
	uint8_t LOL_A_STKY;
	uint8_t LOS_STKY;
};

int Si5351_init(uint8_t, uint32_t, int32_t);
void Si5351_reset(void);
uint8_t set_freq(uint64_t, enum si5351_clock);
uint8_t set_freq_manual(uint64_t, uint64_t, enum si5351_clock);
void set_pll(uint64_t, enum si5351_pll);
void set_ms(enum si5351_clock, struct Si5351RegSet, uint8_t, uint8_t, uint8_t);
void output_enable(enum si5351_clock, uint8_t);
void drive_strength(enum si5351_clock, enum si5351_drive);
void update_status(void);
void set_correction(int32_t, enum si5351_pll_input);
void set_phase(enum si5351_clock, uint8_t);
int32_t get_correction(enum si5351_pll_input);
void pll_reset(enum si5351_pll);
void set_ms_source(enum si5351_clock, enum si5351_pll);
void set_int(enum si5351_clock, uint8_t);
void set_clock_pwr(enum si5351_clock, uint8_t);
void set_clock_invert(enum si5351_clock, uint8_t);
void set_clock_source(enum si5351_clock, enum si5351_clock_source);
void set_clock_disable(enum si5351_clock, enum si5351_clock_disable);
void set_clock_fanout(enum si5351_clock_fanout, uint8_t);
void set_pll_input(enum si5351_pll, enum si5351_pll_input);
void set_vcxo(uint64_t, uint8_t);
void set_ref_freq(uint32_t, enum si5351_pll_input);
uint8_t si5351_write_bulk(uint8_t, uint8_t, uint8_t *);
uint8_t si5351_write(uint8_t, uint8_t);
uint8_t si5351_read(uint8_t);


uint64_t pll_calc(enum si5351_pll, uint64_t, struct Si5351RegSet *, int32_t, uint8_t);
uint64_t multisynth_calc(uint64_t, uint64_t, struct Si5351RegSet *);
uint64_t multisynth67_calc(uint64_t, uint64_t, struct Si5351RegSet *);
void update_sys_status(struct Si5351Status *);
void update_int_status(struct Si5351IntStatus *);
void ms_div(enum si5351_clock, uint8_t, uint8_t);
uint8_t select_r_div(uint64_t *);
uint8_t select_r_div_ms67(uint64_t *);



//*****************************//

//****function prototypes******//

//*****************************//

#endif