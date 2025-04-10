#pragma once


#define Ts_us             5000.0
#define Ts_s (Ts_us / 1000000.0)

#define ENC_PPR           48.0
#define GEAR_RATIO        30.0

#define TIKS_TO_RAD   (2.0 * M_PI / (ENC_PPR * GEAR_RATIO))

#define LPF_ALPHA         1.0
#define R                 16.0
#define Rw                32.0
#define WRobot            82.0
#define FORW_SPEED        10.0
#define CYC_PROG_BUF_SIZE 1
#define FWD_RAD           6.28
#define R_TURN_RAD        3.525
#define L_TURN_RAD        3.525
#define ZERO_RAD          1.00
#define WHEEL_CIRC        100.50
#define L                 -90.0
#define R                 90.0
#define D180              180.0
#define F                 0.0