#pragma once

#include <Arduino.h>

#include "Config.h"
#include "VE.h"
#include "LeftEncoder.h"
#include "RightEncoder.h"

float TUS(float  TETA0)
{
    float Wd0 = TETA0*(WRobot/R);

    return Wd0;
}