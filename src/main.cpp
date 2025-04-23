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
int mess = 1;
int cross = 0;
int f_cross = 0;

int loc_crack[4][5]{
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
};

float read_cross()
{
  float S_l = analogRead(A0);
  float S_r = analogRead(A1);
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

int state = 1;

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

  while(true)
  {
    cross = read_cross();
    drive_line();
    if(cross == 4)
    {
      fwd(0.8);
      stop();
      cross = 0;
      break;
    }
  }
  while(true)
  {
    float S_l = analogRead(A1);
    drive_to_line(Left, 0, 0, 0, 0);
    break;
  }
  while(true)
  {
    drive_line();
    cross = read_cross();
    if(cross == 1)
    {
      cross = 0;
      stop();
      break;
    }
  }
  while(true)
  {
    cross = read_cross();
    turn(-1);
    if(cross == 1)
    {
      cross = 0;
      stop();
      break;
    }
  }
  while(true)
  {
    drive_to_line(Left, 0, 0, 0, 0);
    break;
  }
  while(true)
  {
    cross = read_cross();
    drive_line();
    if(cross == 4)
    {
      fwd(0.8);
      cross = 0;
      stop();
      break;
    }
  }
  while(true)
  {
    float S_r = analogRead(A0);
    drive_to_line(Right, 0, 0, 0, 0);
    break;
  }
  while(true)
  {
    cross = read_cross();
    turn(-1);
    if(cross == 1)
    {
      cross = 0;
      stop();
      break;
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
}
