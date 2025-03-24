#include <Arduino.h>

#include "Config.h"
#include "VE.h"
#include "LeftEncoder.h"
#include "RightEncoder.h"
#include "TUS.h"
#include "TPS.h"
#include "ASMR.h"
#include "Left_Motor.h"
#include "Right_Motor.h"

float or_robot_deg = 0;

void wait()
{
  static int32_t timer = micros();
  while (micros() - timer < Ts_us);
  timer = micros();
}

float left_speed_mind(float err, float max_output)
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

void left_speed_reg(float L_wish_speed)
{
  float left_enc = left_enc_tick();
  float left_w = left_velest_tick();
  float u_l = left_speed_mind(L_wish_speed - left_w, 5);
  l_motor_tick(u_l);
}

float right_speed_mind(float err, float max_output)
{
  const float K = 0.3;
  const float T = 0.01;
  const float Kp = K;
  const float Ki = K / T;

  const float p = err * Kp;
  static float I = 0;
  const float i = I * Ki;
  float ul = p + i;

  if (ul == constrain(ul, -max_output, max_output) || (err * ul) < 0)
  {
    I += err * Ts_s;
  }
  return ul;
}

void right_speed_reg(float R_wish_speed)
{
  float right_enc = right_enc_tick();
  float right_w = right_velest_tick();
  float u_r = right_speed_mind(R_wish_speed - right_w, 5);
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
    left_speed_reg(5);
    right_speed_reg(5);

    if (left_enc >= Target || right_enc >= Target)
    {
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

void stop()
{
  // PLAN
  float u_l = 0;
  float u_r = 0;
  // ACT
  l_motor_tick(u_l);
  r_motor_tick(u_r);
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
    set_or_robot(R);
  }
  else
  {
    set_or_robot(L);
  }
  fwd(abs(targY));
  stop();
}

void setup()
{
  Serial.begin(9600);

  left_enc_init();
  right_enc_init();

  l_motor_init();
  r_motor_init();

  while (true)
  {
    int comm = 0;
    if(Serial.available())
    {
      comm = Serial.parseFloat();
    }
    if(comm != 0)
    {
      drive_to(comm, 0);
    }
  }
}

void loop()
{
  float left_enc = left_enc_tick();
  float right_enc = right_enc_tick();
  float g_right_w = right_velest_tick();
  float g_left_w = left_velest_tick();
  velest_tick();

  // left_vel_estimator(g_left_phi);
  // left_low_pass_filter(left_w_raw);
  // right_vel_estimator(g_right_phi);
  // right_low_pass_filter(right_w_raw);
  // r_motor_tick(g_right_w);
  // l_motor_tick(g_left_w);

  Serial.print(g_left_w);
  Serial.print(" ");
  Serial.println(g_right_w);
}
