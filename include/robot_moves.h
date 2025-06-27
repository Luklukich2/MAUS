#pragma once

#include "Config.h"
#include "LeftEncoder.h"
#include "Left_Motor.h"
#include "RightEncoder.h"
#include "Right_Motor.h"
#include "speed_reg.h"

float or_robot_deg;

void wait()
{
  static int32_t timer = micros();
  while (micros() - timer < Ts_us);
  timer = micros();
}

void turn(float angle_deg)
{
  uint32_t start_time = millis();
  left_enc_zero();
  right_enc_zero();
  float s_L;
  float s_R;
  float K = 0.85;
  if (angle_deg < 0)
  {
    s_L = -5;
    s_R = 5;
  }
  else
  {
    s_L = 5;
    s_R = -5;
  }
  float S = abs(angle_deg) / 360.0 * M_PI * WRobot;
  float N = S / WHEEL_CIRC;
  float phi = (N * (M_PI * 2)) * K;
  while (true)
  {
    // TIMER
    wait();
    // SENSE
    uint32_t time = millis() - start_time;
    float left_enc = left_enc_tick();
    float right_enc = right_enc_tick();
    // PLAN
    // ACT
    left_speed_reg(s_L);
    right_speed_reg(s_R);

    if (abs(right_enc) >= phi)
    {
      break;
    }
  }
}

void set_or_robot(float deg)
{
  float err_orient = deg - or_robot_deg;
  turn(err_orient);
  or_robot_deg += err_orient;
}

void stop()
{
  // PLAN
  float u_l = 0;
  float u_r = 0;
  // ACT
  l_motor_tick(u_l);
  r_motor_tick(u_r);
}
  
void fwd(float targ)
{
  left_enc_zero();
  right_enc_zero();
  uint32_t start_time = millis();
  while (true)
  {
    // TIMER
    wait();
    // SENSE
    uint32_t time = millis() - start_time;
    float cell_size = 6.28;
    float left_enc = left_enc_tick();
    float right_enc = right_enc_tick();
    float Target = cell_size * targ;
    // PLAN
    // ACT
    if (targ < 0)
    {
      left_speed_reg(-5);
      right_speed_reg(-5);
    }
    if (targ > 0)
    {
      left_speed_reg(3);
      right_speed_reg(3);
    }

    if (abs(left_enc) >= abs(Target) || abs(right_enc) >= abs(Target))
    {
      stop();
      break;
    }
  }
}

void right()
{
  left_enc_zero();
  right_enc_zero();
  uint32_t start_time = millis();
  while (true)
  {
    // TIMER
    wait();
    // SENSE
    uint32_t time = millis() - start_time;
    float left_enc = left_enc_tick();
    float right_enc = right_enc_tick();
    // PLAN
    // ACT
    left_speed_reg(5);
    right_speed_reg(-5);

    if (left_enc >= R_TURN_RAD)
    {
      break;
    }
  }
}

void left()
{
  left_enc_zero();
  right_enc_zero();
  uint32_t start_time = millis();
  while (true)
  {
    // TIMER
    wait();
    // SENSE
    uint32_t time = millis() - start_time;
    float left_enc = left_enc_tick();
    float right_enc = right_enc_tick();
    // PLAN
    // ACT
    left_speed_reg(-5);
    right_speed_reg(5);

    if (right_enc >= L_TURN_RAD)
    {
      break;
    }
  }
}


void 
drive_to_line(float side, float cross, float f_cross, float wish_cross, float wish_sect)
{
  uint32_t time0 = millis();

  while (true)
  {
    float S_r = analogRead(A0);
    float S_l = analogRead(A1);
    if (side == Right)
    {
      // Serial.println(S_r);
      left_speed_reg(6);
      right_speed_reg(-6);
      if (S_r > Line_threshold && millis() - time0 > TURN_DELAY)
      {
        stop();
        break;
      }
    }
    if (side == Left)
    {
      left_speed_reg(-6);
      right_speed_reg(6);
      if (S_l > Line_threshold && millis() - time0 > TURN_DELAY)
      {
        stop();
        break;
      }
    }
    if (side == Forward)
    {
      fwd(wish_sect);
      stop();
      if (S_l > Line_threshold && S_r > Line_threshold)
      {
        if (f_cross == 0)
        {
          f_cross = 1;
          cross += 1;
        }
      }
      else
      {
        if (f_cross == 1)
        {
          f_cross = 0;
        }
      }
      if (cross == wish_cross)
      {
        stop();
        break;
      }
    }
  }
  stop();
}

void drive_to(float targX, float targY)
{
  if (targX < 0)
  {
    set_or_robot(D180);
    stop();
  }
  else
  {
    set_or_robot(0);
    stop();
  }
  fwd(abs(targX));
  if (targY < 0)
  {
    set_or_robot(Right);
  }
  else
  {
    set_or_robot(Left);
  }
  fwd(abs(targY));
  stop();
}