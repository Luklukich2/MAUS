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
#include "RF24.h"
#include "RF24_config.h"
#include "uMQ.h"

float x_or_robot = 0;
float y_or_robot = 0;
int comm1 = 0;
int comm2 = 0;
int cross = 0;
int f_cross = 0;
int orX = 1;
int orY = 0;
float H_sens = 1;
String data;
String mess;
String addres;
String send_data;

int loc_crack[2][4]{
    1, 1, 1, 1,
    1, 1, 1, 1,   
};

uMQ radio;

float read_cross()
{
  float S_l = analogRead(A0);
  float S_r = analogRead(A1);
  if(S_l >= 650 && S_r >= 650)
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

void hole_read(int orX, int orY)
{
  while(true)
  {
    H_sens = digitalRead(A5);
    loc_crack[orX][orY] = H_sens;
    break;
  }
}

void drive_cross(float wish_cross)
{
  while(true)
  {
    drive_line();
    cross = read_cross();
    if(cross == wish_cross)
    {
      stop();
      hole_read(orX, orY);
      break;
    }
  }
}

// void drive_cross(float wish_cross)
// {
//   cross = read_cross();
//     drive_line();
//     if(cross == wish_cross)
//     {
//       fwd(0.7);
//       stop();
//       cross = 0;
//       break;
//     }
// }

int state = 0;

void setup()
{
  Serial.begin(9600);

  left_enc_init();
  right_enc_init();

  l_motor_init();
  r_motor_init();

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  radio.init(A2, A3);
  
  // while(true)
  // {
  //   Serial.println("Ожидаю данных...");
  //   data = radio.recv("ALESH", 100);
  //   Serial.println(data);
  //   if(data != "")
  //   {
  //     mess = data.substring(6);
  //     addres = data.substring(0, 5);
  //     break;
  //   }
  // }
  // if (mess == "start")
  // {
    while(true) // проезд по 1 ряду
    {
      drive_line();
      cross = read_cross();
      if(digitalRead(A5) == 0)
      {
        hole_read(orX, orY);
        stop();
        break;
      }
    // }
    send_data[0] = orX;
    send_data[1] = orY;
    radio.send(send_data, "DOBRY", 100);
  }
  // while(true) // съезд на промежуток 12
  // {
  //   drive_to_line(Left, 0, 0, 0, 0);
  //   break;
  // }
  // while(true) // проезд по промжутку 12
  // {
  //   right_speed_reg(5);
  //   delay(970);
  //   stop();
  //   break;
  // }
  // while(true) // доворот с промежутка
  // {
  //   drive_cross(1);
  //   fwd(0.7);
  //   stop();
  //   break;
  // }
  // while(true) // поворот на 2 ряд
  // {
  //   drive_to_line(Left, 0, 0, 0, 0);
  //   break;
  // }
  // while(true) // проезд по 2 ряду
  // {
  //   drive_cross(5);
  //   fwd(0.7);
  //   stop();
  //   break;
  // }
  // while(true) // съезд на промежуток 23
  // {
  //   drive_to_line(Right, 0, 0, 0, 0);
  //   break;
  // }
  // while(true) // доворот на промежуток 23
  // {
  //   left_speed_reg(5);
  //   delay(870);
  //   stop();
  //   break;
  // }
  // while(true) // проезд по промежутку 23
  // {
  //   drive_cross(1);
  //   fwd(0.7);
  //   stop();
  //   break;
  // }
  // while(true) // доезд с промежутка 23
  // {
  //   left_speed_reg(5);
  //   delay(870);
  //   stop();
  //   break;
  // }
  // while(true) // поворот на 3 ряд
  // {
  //   drive_to_line(Right, 0, 0, 0, 0);
  //   break; 
  // }
  // while(true) // проезд по 3 ряду
  // {
  //   drive_cross(4);
  //   fwd(0.7);
  //   stop();
  //   break;
  // }
  // while(true)
  // {
  //    drive_to_line(Right, 0, 0, 0, 0);
  //    break;
  // }
  // while(true)
  // {
  //   left_speed_reg(5);
  //   delay(870);
  //   stop();
  //   break;
  // }
  // while(true)
  // {
  //   drive_cross(2);
  //   fwd(0.7);
  //   stop();
  //   break;
  // }
  // while(true)
  // {
  //   drive_to_line(Right, 0, 0, 0, 0);
  //   break;
  // }
  // while(true)
  // {
  //   left_speed_reg(5);
  //   delay(870);
  //   stop();
  //   break;
  // }
  // while(true)
  // {
  //   drive_cross(4);
  //   fwd(0.7);
  //   stop();
  //   break;
  // }
  // }
}

void loop()
{
  float left_enc = left_enc_tick();
  float right_enc = right_enc_tick();
  float g_right_w = right_velest_tick();
  float g_left_w = left_velest_tick();
  float H_sens = digitalRead(A5);
  float S_l = analogRead(A1);
  float S_r = analogRead(A0);
  velest_tick();
  Serial.print(S_l);
  Serial.print(" ");
  Serial.println(S_r);
  // drive_line();
}