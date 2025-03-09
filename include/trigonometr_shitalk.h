#include "VE.h"
#include "Config.h"

float encL = g_left_phi;
float encR = g_right_phi;

float D_phiL = encL - Old_phiL;
float D_phiR = encR - Old_phiR;

float D_Sr;
float D_Sl;
   
float D_Slo(float D_phiL) {
    D_Sl = D_phiL*Rw;
    return D_Sl;
}

float D_Sro(float D_phiR) {
    D_Sr = D_phiR*Rw;
    return D_Sr;
}

float D_Sf = (D_Sr + D_Sl) / 2;
float D_teta = (D_Sr*D_Sl) / WRobot;

float Old_phiL = g_left_phi;
float Old_phiR = g_right_phi;
