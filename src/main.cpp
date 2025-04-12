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
#include "speed_reg.h"
#include "Line.h"
#include "robot_moves.h"
// #include "Radio.h"
// #include "RF24.h"
// #include "RF24_config.h"

float x_or_robot = 0;
float y_or_robot = 0;
int comm1 = 0;
int comm2 = 0;
float S_r = 0;
float S_l = 0;
int mess = 1;
int cross = 0;
int f_cross = 0;

void setup()
{
  Serial.begin(9600);

  left_enc_init();
  right_enc_init();

  l_motor_init();
  r_motor_init();

  // radio_setup();
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);

  // while (true)
  // {
  //   if(Serial.available())
  //   {
  //     comm1 = Serial.parseFloat(); comm2 = Serial.parseFloat();
  //   }
  //   if(comm1 != 0 || comm2 != 0)
  //   {
  //     float X = (comm1 - x_or_robot);
  //     float Y = (comm2 - y_or_robot);
  //     drive_to(X, Y);
  //   } 
  // }
}

void loop()
{
  float left_enc = left_enc_tick();
  float right_enc = right_enc_tick();
  float g_right_w = right_velest_tick();
  float g_left_w = left_velest_tick();
  S_r = analogRead(A2);
  S_l = analogRead(A1);
  velest_tick();

  drive_to_line(Forward, 0, 0);
  Serial.print(f_cross);
  Serial.print(" ");
  Serial.println(cross);
  // left_vel_estimator(g_left_phi);
  // left_low_pass_filter(left_w_raw);
  // right_vel_estimator(g_right_phi);
  // right_low_pass_filter(right_w_raw);
  // r_motor_tick(g_right_w);
  // l_motor_tick(g_left_w);

  // Serial.print(S_r);
  // Serial.print(" ");
  // Serial.println(S_l);
}
