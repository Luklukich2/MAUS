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
  init0,
  tier0,
  home0,
  tier1,
  home1,
  tier2,
  home2,
  tier3,
  home3,
  write_data,
  stop0,
};

int state = init0;

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

  switch (state)
  {
  case init0:
    fwd(0.6);
    stop();
    set_or_robot(180);
    fwd(0.6);
    set_or_robot(0);
    stop();
    state = tier0;
  case tier0:
    left_speed_reg(2.5);
    right_speed_reg(2.5);
    if (S_l >= 870 && S_r >= 870)
    {
      stop();
      loc_crack[2][1] = 1;
    }
    state = 45;
  // case home0:
  //   drive_to(-3, 1);
  //   right();
  //   state = 45;
  // case tier1:
  //   drive_to_line(Forward, 0, 0, 1, 1);
  //   if (cross == 1)
  //   {
  //     loc_crack[3][1] = 1;
  //   }
  //   drive_to_line(Forward, 0, 0, 1, 1);
  //   if (cross == 1)
  //   {
  //     loc_crack[3][2] = 1;
  //   }
  //   drive_to_line(Forward, 0, 0, 1, 1);
  //   if (cross == 1)
  //   {
  //     loc_crack[3][3] = 1;
  //   }
  //   drive_to_line(Forward, 0, 0, 1, 1);
  //   if (cross == 1)
  //   {
  //     loc_crack[3][4] = 1;
  //   }
  //   state = home1;
  // case home1:
  //   drive_to(-4, 1);
  //   right();
  //   state = tier2;
  // case tier2:
  //   drive_to_line(Forward, 0, 0, 1, 1);
  //   if (cross == 1)
  //   {
  //     loc_crack[2][1] = 1;
  //   }
  //   drive_to_line(Forward, 0, 0, 1, 1);
  //   if (cross == 1)
  //   {
  //     loc_crack[2][2] = 1;
  //   }
  //   drive_to_line(Forward, 0, 0, 1, 1);
  //   if (cross == 1)
  //   {
  //     loc_crack[2][3] = 1;
  //   }
  //   drive_to_line(Forward, 0, 0, 1, 1);
  //   if (cross == 1)
  //   {
  //     loc_crack[2][4] = 1;
  //   }
  //   state = home2;
  // case home2:
  //   drive_to(-4, 1);
  //   right();
  //   state = tier3;
  // case tier3:
  //   drive_to_line(Forward, 0, 0, 1, 1);
  //   if (cross == 1)
  //   {
  //     loc_crack[1][1] = 1;
  //   }
  //   drive_to_line(Forward, 0, 0, 1, 1);
  //   if (cross == 1)
  //   {
  //     loc_crack[1][2] = 1;
  //   }
  //   drive_to_line(Forward, 0, 0, 1, 1);
  //   if (cross == 1)
  //   {
  //     loc_crack[1][3] = 1;
  //   }
  //   drive_to_line(Forward, 0, 0, 1, 1);
  //   if (cross == 1)
  //   {
  //     loc_crack[1][4] = 1;
  //   }
  //   state = home3;
  // case home3:
  //   drive_to(-4, -3);
  //   right();
  //   state = write_data;
  // case write_data:
  //   Serial.println(loc_crack[2][1]);
  //   state = stop0;
  case stop0:
    stop();
  }
}
