#pragma once

#include <Arduino.h>
#include "Config.h"
#include "TUS.h"
#include "TPS.h"


#define RIGHT_CLK_A_PIN 3
#define RIGHT_B_PIN 5

#define RIGHT_ENC_DIR 1

void right_enc_handler();

volatile int g_right_counter = 0;
float g_right_phi = 0;
int8_t g_right_ett[4][4] = {0};


void right_enc_init()
{
    // Настройка пинов энкодера на вход
    pinMode(RIGHT_CLK_A_PIN, INPUT);
    pinMode(RIGHT_B_PIN, INPUT);

    // Отключение прерываний
    noInterrupts();
    // Подключение обработчика прерывания энкодера
    attachInterrupt(
        digitalPinToInterrupt(RIGHT_CLK_A_PIN),
        right_enc_handler,
        CHANGE
    );

    // Настройка таблицы переходов
    g_right_ett[0b00][0b01] = RIGHT_ENC_DIR;
    g_right_ett[0b01][0b11] = RIGHT_ENC_DIR;
    g_right_ett[0b11][0b10] = RIGHT_ENC_DIR;
    g_right_ett[0b10][0b00] = RIGHT_ENC_DIR;
    // 13 == 0xD == 0b1101 == 015;
    g_right_ett[0b00][0b10] = -RIGHT_ENC_DIR;
    g_right_ett[0b10][0b11] = -RIGHT_ENC_DIR;
    g_right_ett[0b11][0b01] = -RIGHT_ENC_DIR;
    g_right_ett[0b01][0b00] = -RIGHT_ENC_DIR;
    
    
    interrupts();
}

void right_enc_handler()
{
    static uint8_t right_enc_old = 0;

    const uint8_t RIGHT_B = digitalRead(RIGHT_B_PIN);
    // 0000000B
    
    const uint8_t RIGHT_CLK_A = digitalRead(RIGHT_CLK_A_PIN);
    const uint8_t RIGHT_A = RIGHT_CLK_A ^ RIGHT_B;
    // 0000000A

    const uint8_t right_enc = (RIGHT_A << 1) | RIGHT_B;
    /*
    000000AB = (0000000A << 1) | 0000000B;
    */

    g_right_counter += g_right_ett[right_enc_old][right_enc];
    right_enc_old = right_enc;
}

void right_enc_tick()
{
    noInterrupts();
    const int right_counter_inc = g_right_counter;
    g_right_counter = 0;
    interrupts();

    g_right_phi += right_counter_inc * TIKS_TO_RAD;
}