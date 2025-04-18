#pragma once

#include <Arduino.h>

#include "Right_Motor.h"
#include "Left_Motor.h"
#include "VE.h"

float speed_mind(float err, float max_output)
{
  const float K = 0.3;
  const float T = 0.01;
  const float Kp = K;
  const float Ki = K / T;

  const float p = err * Kp;
  static float I = 0;
  const float i = I * Ki;
  float ur = p + i;

  if (ur == constrain(ur, -max_output, max_output) || (err * ur) < 0)
  {
    I += err * Ts_s;
  }
  return ur;
}
// 
void left_speed_reg(float L_wish_speed)
{
  float left_enc = left_enc_tick();
  float left_w = left_velest_tick();
  float u_l = speed_mind(L_wish_speed - left_w, 5);
  l_motor_tick(u_l);
}

void right_speed_reg(float R_wish_speed)
{
  float right_enc = right_enc_tick();
  float right_w = right_velest_tick();
  float u_r = speed_mind(R_wish_speed - right_w, 5);
  r_motor_tick(u_r);
}