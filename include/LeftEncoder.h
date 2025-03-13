#pragma once


#include "Config.h"
#include "TUS.h"
#include "TPS.h"

#define LEFT_CLK_A_PIN 2
#define LEFT_B_PIN 4

#define LEFT_ENC_DIR -1

#define ENC_PWR 12
#define GEAR_RATIO 30

void left_enc_handler();

volatile int g_left_counter = 0;
float g_left_phi = 0;
int8_t g_left_ett[4][4] = {0};

const float tick_to_rad = 2.0 * M_PI / (ENC_PPR * GEAR_RATIO);

void left_enc_init()
{
    // Настройка пинов энкодера на вход
    pinMode(LEFT_CLK_A_PIN, INPUT);
    pinMode(LEFT_B_PIN, INPUT);

    // Отключение прерываний
    noInterrupts();
    // Подключение обработчика прерывания энкодера
    attachInterrupt(
        digitalPinToInterrupt(LEFT_CLK_A_PIN),
        left_enc_handler,
        CHANGE
    );

    // Настройка таблицы переходов
    g_left_ett[0b00][0b01] = LEFT_ENC_DIR;
    g_left_ett[0b01][0b11] = LEFT_ENC_DIR;
    g_left_ett[0b11][0b10] = LEFT_ENC_DIR;
    g_left_ett[0b10][0b00] = LEFT_ENC_DIR;
    // 13 == 0xD == 0b1101 == 015;
    g_left_ett[0b00][0b10] = -LEFT_ENC_DIR;
    g_left_ett[0b10][0b11] = -LEFT_ENC_DIR;
    g_left_ett[0b11][0b01] = -LEFT_ENC_DIR;
    g_left_ett[0b01][0b00] = -LEFT_ENC_DIR;
    
    interrupts();
}

void left_enc_handler()
{
    static uint8_t left_enc_old = 0;

    const uint8_t LEFT_B = digitalRead(LEFT_B_PIN);
    // 0000000B
    
    const uint8_t LEFT_CLK_A = digitalRead(LEFT_CLK_A_PIN);
    const uint8_t LEFT_A = LEFT_CLK_A ^ LEFT_B;
    // 0000000A

    const uint8_t left_enc = (LEFT_A << 1) | LEFT_B;
    /*
    000000AB = (0000000A << 1) | 0000000B;
    */

    g_left_counter += g_left_ett[left_enc_old][left_enc];
    left_enc_old = left_enc;
}

float left_enc_tick()
{
    noInterrupts();
    const int left_counter_inc = g_left_counter;
    g_left_counter = 0;
    interrupts();

    g_left_phi += left_counter_inc * TIKS_TO_RAD;

    return g_left_phi;
}

void left_enc_zero()
{
    g_left_phi = 0;
}