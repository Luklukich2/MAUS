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
float left_enc = 0;
float right_enc = 0;

String data;
String send_data = "NO FAULT DETECTED";
String mess;
String answer;

int loc_crack[5][5]{
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
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
    drive_line(0.5);
    if(digitalRead(A5) == 0) // проверка датчика Холла
    {
      while(true)
      {
        stop(); // остановка моторов
        hole_read(orX, orY); // записывание координат поломки
        send_data = "*CORDS:" + String(orX) + ":" + String(orY); // формирование строки для отправки
        radio.send(send_data, "SCADA", 100); // посылка строки
        delay(100);
        answer = radio.recv("ALESH", 100);
        if(answer == "okay")
        {
          fwd(0.4);
          stop();
          break;
        }
        delay(100);
      }
      // CORDS: X: 3, Y: 1 // формат сообщения
      break;
    }
    if(cross == wish_cross)
    {
      stop();
      cross = 0;
      break;
    }
  }
  stop();
}

void turn90L(int side)
{
  fwd(0.5);
  stop();
  left_enc_zero();
  right_enc_zero();
  if(side == Left)
  {
    while(true)
    {
      left_enc = left_enc_tick();
      left_speed_reg(0.5);
      if(left_enc >= 6.5)
      {
        stop();
        break;
      }
    }
  }
  if(side == Right)
  {
    while(true)
    {
      right_enc = right_enc_tick();
      right_speed_reg(0.5);
      if(right_enc >= 6.5)
      {
        stop();
        break;
      }
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

  pinMode(A1, INPUT);
  pinMode(A2, INPUT);

  left_speed_reg(0);
  right_speed_reg(0);

  Serial.println("INIT COMPLETE");


  
  // while(true)
  // {
  //   left_speed_reg(7);
  //   right_speed_reg(-7);
  // }
  // while(true)
  // {
  //   radio.send("1234", "DOBRY", 100);
  // }
  while(true)
  {
    data = radio.recv("ALESH", 100);
    data.trim();
    Serial.println(data);
    if (data == "start")
    {
      // Serial.println("START RECVD");
      break;
    }
  }
  radio.send("PROG START", "SCADA", 100);
  for(int i = 0; i < 4; i++)
  {
    drive_cross(1);
    orX++;
  }
  turn90L(Left);
  fwd(-0.6);
  drive_cross(1);
  orY++;
  turn90L(Left);
  fwd(-0.6);
  for(int i = 0; i < 4; i++)
  {
    drive_cross(1);
    orX--;
  }
  turn90L(Right);
  fwd(-0.6);
  drive_cross(1);
  orY++;
  turn90L(Right);
  fwd(-0.6);
  for(int i = 0; i < 4; i++)
  {
    drive_cross(1);
    orX++;
  }
  fwd(0.6);
  while(true)
  {
    // radio.send(send_data, "SCADA", 50);
    // Serial.println(send_data);
  }
}

void loop()
{
  // float S_r = analogRead(A0);
  // float S_l = analogRead(A1);
  float left_enc = left_enc_tick();
  float right_enc = right_enc_tick();
  float g_right_w = right_velest_tick();
  float g_left_w = left_velest_tick();
  velest_tick();
  // Serial.println(String(S_r) + " " + String(S_l));
}