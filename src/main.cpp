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
    float left_enc = left_enc_tick();
    float right_enc = right_enc_tick();
    uint32_t time = millis() - start_time;
    // PLAN
    float u_l = 5.5;
    float u_r = 5.5;
    // ACT
    l_motor_tick(u_l);
    r_motor_tick(u_r);

    if (left_enc >= 6.28 || right_enc >= 6.28)
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
    float left_enc = left_enc_tick();
    float right_enc = right_enc_tick();
    uint32_t time = millis() - start_time;
    // PLAN
    float u_l = 5.5;
    float u_r = 0;
    // ACT
    l_motor_tick(u_l);
    r_motor_tick(u_r);

    if (left_enc >= 7.05)
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
    float err_tl = L_TURN_RAD - g_right_phi;
    float ur = left_speed_mind(err_tl, 9);
    float left_enc = left_enc_tick();
    float right_enc = right_enc_tick();
    uint32_t time = millis() - start_time;
    // PLAN
    float u_l = 0;
    float u_r = ur;
    // ACT
    l_motor_tick(u_l);
    r_motor_tick(u_r);

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
  // velest_tick();

  // left_vel_estimator(g_left_phi);
  // left_low_pass_filter(left_w_raw);
  // right_vel_estimator(g_right_phi);
  // right_low_pass_filter(right_w_raw);
  // r_motor_tick(g_right_w);
  // l_motor_tick(g_left_w);

  Serial.print(g_left_phi);
  Serial.print(" ");
  Serial.println(g_right_phi);
}
