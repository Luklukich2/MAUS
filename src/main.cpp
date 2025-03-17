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

void wait()
{
  static int32_t timer = micros();
  while (micros() - timer < Ts_us);
  timer = micros();
}


float left_speed_mind(float err, float max_output)
{
    const float K = 3;
    const float T = 0.01;
    const float Kp = K;
    const float Ki = K / T;

    const float p = err * Kp;
    static float I = 0;
    const float i = I * Ki;
    float ur = p * i;

    if (ur == constrain(ur, -max_output, max_output) || (err * ur) < 0)
    {
       I+= err * Ts_s;
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
    const float K = 3;
    const float T = 0.01;
    const float Kp = K;
    const float Ki = K / T;

    const float p = err * Kp;
    static float I = 0;
    const float i = I * Ki;
    float ul = p * i;

    if (ul == constrain(ul, -max_output, max_output) || (err * ul) < 0)
    {
       I+= err * Ts_s;
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


void fwd()
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
    right_speed_reg(5);

    if (left_enc >= FWD_RAD || right_enc >= FWD_RAD)
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
    right_speed_reg(0);

    if (left_enc >= R_TURN_RAD || right_enc >= 0)
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
    left_speed_reg(0);
    right_speed_reg(5);

    if (left_enc >= ZERO_RAD || right_enc >= L_TURN_RAD)
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

void setup()
{
  Serial.begin(9600);

  left_enc_init();
  right_enc_init();

  l_motor_init();
  r_motor_init();

  fwd();
  left();
  fwd();
  left();
  fwd();
  left();
  fwd();
  left();
  stop();

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
