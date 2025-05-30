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
#include "uMQ.h"
// #include "Radio.h"
// #include "RF24.h"
// #include "RF24_config.h"

uMQ radio;

int orX = 0;
int orY = 0;
float x_or_robot = 0;
float y_or_robot = 0;
float H_sens = 1;
int comm1 = 0;
int comm2 = 0;
int cross = 0;
int f_cross = 0;

String data;
String send_data = "NO FAULT DETECTED";
String mess;

int loc_crack[3][5]{
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
};

int read_cross()
{
  float S_l = analogRead(A0);
  float S_r = analogRead(A1);
  // Serial.println(String(S_l) + " " + String(S_r));
  if(S_l >= Line_threshold && S_r >= Line_threshold)
  {
    if(f_cross == 0)
    {
      f_cross = 1;
      cross += 1;
      Serial.println("cross = " + String(cross));
    }
  }else{
    if(f_cross == 1)
    {
      f_cross = 0;
    }
  }
  return cross;
}

void hole_read(int orX, int orY)
{
  H_sens = digitalRead(A5);
  loc_crack[orX][orY] = H_sens;
}

void drive_cross(int wish_cross)
{
  Serial.println("drive_cross(" + String(wish_cross) + ")");
  while(true)
  {
    cross = read_cross();
    drive_line();
    if(digitalRead(A5) == 0)
    {
      stop();
      // Serial.println("MAGNET ACSESS");
      hole_read(orX, orY);
      send_data = "X: " + String(orX) + ", Y: " + String(orY);
      // Serial.println(send_data);
      // X: 3, Y: 1
    }
    if(cross == wish_cross)
    {
      fwd(0.7);
      stop();
      cross = 0;
      break;
    }
  }
}



int state = 1;

void setup()
{
  Serial.begin(9600);

  radio.init(A2, A3);

  left_enc_init();
  right_enc_init();

  l_motor_init();
  r_motor_init();

  // radio_setup();
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);

  left_speed_reg(0);
  right_speed_reg(0);

  Serial.println("INIT COMPLETE");


  // while(true)
  // {
  //   radio.send("1234", "DOBRY", 100);
  // }

  radio.send("PROG START", "DOBRY", 100);
  while(true)
  {
    data = radio.recv("ALESH", 100);
    data.trim();
    Serial.println(data);
    if (data == "start")
    {
      Serial.println("START RECVD");
      break;
    }
  }
  radio.send("ALESH GO", "DOBRY", 100);
  for(int i = 0; i < 4; i++)
  {
    drive_cross(1);
    orX += 1;
  }

  drive_to_line(Left, 0, 0, 0, 0);
  // проезд по промжутку 12
  drive_cross(1);
  orY += 1;
  // поворот на 2 ряд
  drive_to_line(Left, 0, 0, 0, 0);

  for(int i = 0; i < 4; i++)
  {
    drive_cross(1);
    orX -= 1;
  }

  // съезд на промежуток 23
  drive_to_line(Right, 0, 0, 0, 0);
  // проезд по промежутку 23
  drive_cross(1);
  orY += 1;
  // поворот на 3 ряд
  drive_to_line(Right, 0, 0, 0, 0);
  // проезд по 3 ряду
  for(int i = 0; i < 4; i++)
  {
    drive_cross(1);
    orX += 1;
  }
  drive_to_line(Right, 0, 0, 0, 0);
  drive_cross(1);
  drive_cross(1);
  drive_to_line(Right, 0, 0, 0, 0);
  drive_cross(1);
  drive_cross(1);
  drive_cross(1);
  drive_cross(1);
  stop();

  while(true)
  {
    radio.send(send_data, "DOBRY", 50);
    // Serial.println(send_data);
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