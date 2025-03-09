#pragma once

#include <Arduino.h>
#define R_MOTOR_IN 5
#define R_MOTOR_PWM 10
#define R_MOTOR_DIR 8
#define SUPPLY_VOLTAGE 12

// Инициализация мотора:

void r_motor_init()
{
  pinMode(R_MOTOR_IN, OUTPUT);
  pinMode(R_MOTOR_PWM, OUTPUT);
}

// Выдача напряжения:

void r_motor_tick(float u)
{
  const int16_t pwm = 255.0 * constrain(u / SUPPLY_VOLTAGE, -1.0, 1.0) * R_MOTOR_DIR;

  if (pwm >= 0)
  {
    digitalWrite(R_MOTOR_IN, LOW);
    analogWrite(R_MOTOR_PWM, pwm);
  }
  else
  {
    digitalWrite(R_MOTOR_IN, HIGH);
    analogWrite(R_MOTOR_PWM, 255 + pwm);
  }
}