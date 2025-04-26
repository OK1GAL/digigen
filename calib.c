#include "header.h"

void start_freq_calib(uint64_t real_freq)
{
    
}

long int calib_offset = 0;
void calibrate_freq(uint64_t real_freq)
{
    calib_offset = (long int)current_center_freq - (long int)real_freq;
}