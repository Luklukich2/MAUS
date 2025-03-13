#pragma once


#define Ts_us 5000
#define Ts_s (Ts_us / 1000000.0)

#define ENC_PPR 48
#define GEAR_RATIO 30

#define TIKS_TO_RAD   (2.0 * M_PI / (ENC_PPR * GEAR_RATIO))

#define LPF_ALPHA 1
#define R  16
#define Rw  32
#define WRobot 80
#define FORW_SPEED 10
#define CYC_PROG_BUF_SIZE 1
#define FWD_RAD           6.28
#define R_TURN_RAD        7.05
#define L_TURN_RAD        7.05