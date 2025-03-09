#pragma once

#include <Arduino.h>
#define L_MOTOR_IN 7
#define L_MOTOR_PWM 9
#define L_MOTOR_DIR -1
#define SUPPLY_VOLTAGE 12

// Инициализация мотора:

void l_motor_init()
{
  pinMode(L_MOTOR_IN, OUTPUT);
  pinMode(L_MOTOR_PWM, OUTPUT);
}

// Выдача напряжения:

void l_motor_tick(float u)
{
  const int16_t pwm = 255.0 * constrain(u / SUPPLY_VOLTAGE, -1.0, 1.0) * L_MOTOR_DIR;

  if (pwm >= 0)
  {
    digitalWrite(L_MOTOR_IN, LOW);
    analogWrite(L_MOTOR_PWM, pwm);
  }
  else
  {
    digitalWrite(L_MOTOR_IN, HIGH);
    analogWrite(L_MOTOR_PWM, -pwm);
  }
}