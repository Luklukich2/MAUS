#pragma once

#include <Arduino.h>

#include "Config.h"
#include "VE.h"
#include "LeftEncoder.h"
#include "RightEncoder.h"

float TPS(float Uf0, float R0)
{
    float Wf0 = Uf0/R0;

    return Wf0;
}