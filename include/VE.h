#pragma once

#include "Config.h"
#include "LeftEncoder.h"
#include "RightEncoder.h"

//лево VE
float left_vel_estimator(float left_phi)
{
    static float phi_old = 0;
    float left_w_raw = (left_phi - phi_old) / Ts_s;
    phi_old = left_phi;
    return left_w_raw;
}
float left_low_pass_filter(float left_w_raw)
{
    static float left_w = 0;
    left_w += (left_w_raw - left_w) * LPF_ALPHA;

    return left_w;
}

//право VE
float right_vel_estimator(float right_phi)
{
    static float right_phi_old = 0;
    float  right_w_raw= (right_phi - right_phi_old) / Ts_s;
    right_phi_old = right_phi;
    return right_w_raw;
}
float right_low_pass_filter(float rigth_w_raw)
{
    static float right_w = 0;
    right_w += (rigth_w_raw - right_w) * LPF_ALPHA;

    return right_w;
}

float g_left_w;
float g_right_w;
float left_w_raw;
float right_w_raw;

float left_velest_tick()
{
    const float left_phi = g_left_phi;
    const float left_w_raw = left_vel_estimator(left_phi);
    g_left_w = left_low_pass_filter(left_w_raw);

    return g_left_w;
}

float right_velest_tick()
{
    const float right_phi = g_right_phi;
    const float right_w_raw = right_vel_estimator(right_phi);
    g_right_w = right_low_pass_filter(right_w_raw);

    return g_right_w;
}


void velest_tick()
{
    const float left_phi = g_left_phi;
    const float right_phi = g_right_phi;

    const float left_w_raw = left_vel_estimator(left_phi);
    const float right_w_raw = right_vel_estimator(right_phi);

    g_left_w = left_low_pass_filter(left_w_raw);
    g_right_w = right_low_pass_filter(right_w_raw);
}

// float vel_pi_reg(float err, float max_output)
// {
//     const float K = 3;
//     const float T = 0.01;
//     const float Kp = K;
//     const float Ki = K / T;

//     const float p = err * Kp;
//     static float I = 0;
//     const float i = I * Ki;
//     float u = p * i;

//     if (u == constrain(u, -max_output, max_output) || (err * u) < 0)
//     {
//        I+= err * Ts_s;
//     }
//     return u;
// }

