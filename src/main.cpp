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

int loc_crack[4][5]{
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
};

enum
{
  tier0,
  turn0,
  home0,
  tier1,
  home1,
  tier2,
  home2,
  tier3,
  home3,
  stop0,
};

float read_cross()
{
  if(S_l >= 630 && S_r >= 630)
  {
    if(f_cross == 0)
    {
      f_cross = 1;
      cross += 1;
    }
  }else{
    if(f_cross == 1)
    {
      f_cross = 0;
    }
  }
  return cross;
}

int state = tier0;

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
  S_r = analogRead(A0);
  S_l = analogRead(A1);
  velest_tick();

  switch (state)
  {
  case tier0:
    drive_line(S_r, S_l);
    cross = read_cross();
    if (cross == 4)
    {
      stop();
      state = turn0;
      break;
    }
  case turn0:
    drive_to_line(Left, 0, 0, 0, 0);
    state = 45;
    break;
  default:
    break;
  }
}
