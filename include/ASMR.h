#include <Arduino.h>

#include "Config.h"
#include "VE.h"
#include "LeftEncoder.h"
#include "RightEncoder.h"
#include "TUS.h"
#include "TPS.h"
#include <inttypes.h>

struct Sensors
{
    float time;
};

struct MotionState
{
    float v_f0;
    float theta_i0;
    bool is_completed;
};

typedef void (*Cyclogram)(MotionState*, Sensors);
#define CYCLOGRAM(name) void name(MotionState *ms, Sensors s)

CYCLOGRAM(STOP)
{
    ms->is_completed = false;
    ms->v_f0 = 0;
    ms->theta_i0= 0;
}

CYCLOGRAM(IDLE)
{
    ms->is_completed = true;
    ms->v_f0 = 0;
    ms->theta_i0 = 0;
}

CYCLOGRAM(FWD)
{
    //математика
    ms->v_f0 = FORW_SPEED;
    ms->theta_i0 = 0;

    //логика перехода
    if (s.time >= 1.8)
    {
        ms->is_completed = true;
    }
}

CYCLOGRAM(SS90E)
{
    int dir = 1;

    //Математика
    ms->v_f0 = 0.1;

    if (s.time >= 0.225 && s.time < 1.285)
    {
        ms->theta_i0 = 1.48 * dir;
    }
    else
    {
        ms->theta_i0 = 0;
    }

    //Логика перехода
    if(s.time >= 1.51)
    {
        ms->is_completed = true;
    }
}

class ASMR
{
private:
    Cyclogram
cycProgram[CYC_PROG_BUF_SIZE] = {IDLE};
    int cycProgCounter = 0;
    int cycEnd = 0;

    uint32_t lastProgStart = 0;

    int rotmod(int in)
    {
        return in % CYC_PROG_BUF_SIZE;
    }

    void incProgCounter()
    {
        cycProgCounter++;
        cycProgCounter %= CYC_PROG_BUF_SIZE;
    }


public:
    void addAction(Cyclogram cyc)
    {
        cycEnd = rotmod(cycEnd  + 1);
        cycProgram[cycEnd] = cyc;
    }

    void exec()
    {
        Sensors s = {0};
        MotionState ms = {0};

        s.time = millis() / 1000.0 - lastProgStart;

        Serial.print(s.time);
        Serial.print(" ");

        cycProgram[cycProgCounter](&ms, s);

        if(ms.is_completed)
        {
            cycProgCounter = rotmod(cycProgCounter + 1);
            lastProgStart = millis() / 1000.0;
        }

        // Drive atspeeds
        Serial.println(String(ms.v_f0) + " " + String(ms.theta_i0) + " " + String(ms.is_completed));
    }
};

