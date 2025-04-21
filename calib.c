#include "header.h"

long int calib_offset = 0;
void calibrate(uint64_t real_freq)
{
    calib_offset = (long int)current_center_freq - (long int)real_freq;
}