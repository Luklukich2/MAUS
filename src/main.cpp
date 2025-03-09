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
  while (micros() - timer < Ts_us)
    ;
  timer = micros();
}

void fwd()
{
  uint32_t start_time = millis();
  while (true)
  {
    // TIMER
    wait();
    // SENSE
    uint32_t time = millis() - start_time;
    // PLAN
    float u_l = 5.5;
    float u_r = 5.5;
    // ACT
    l_motor_tick(u_l);
    r_motor_tick(u_r);

    if (time > 650)
    {
      break;
    }
  }
}

void right()
{
  uint32_t start_time = millis();
  while (true)
  {
    // TIMER
    wait();
    // SENSE
    uint32_t time = millis() - start_time;
    // PLAN
    float u_l = 5.5;
    float u_r = -5.5;
    // ACT
    l_motor_tick(u_l);
    r_motor_tick(u_r);

    if (time > 178)
    {
      break;
    }
  }
}

void left()
{
  uint32_t start_time = millis();
  while (true)
  {
    // TIMER
    wait();
    // SENSE
    uint32_t time = millis() - start_time;
    // PLAN
    float u_l = -5.5;
    float u_r = 5.5;
    // ACT
    l_motor_tick(u_l);
    r_motor_tick(u_r);

    if (time > 182)
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
  right();
  fwd();
  right();
  fwd();
  right();
  fwd();
  right();
  stop();
}

void loop()
{

  left_enc_tick();
  right_enc_tick();
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
