#pragma once

#include "Config.h"
#include "speed_reg.h"

void drive_line(float k, float S_r, float S_l)
{
    float u = 0;
    float kef = 0.002;
    float err = S_r - S_l;
    u = err*kef;
    right_speed_reg(1.5 - u);
    left_speed_reg(1.5 + u);
}