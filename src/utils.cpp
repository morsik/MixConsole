#include <cmath>

#include "utils.h"

int iec_scale(float dB, int size)
{
    float def = 0.0f;

    if (dB < -70.0f)
    {
        def = 0.0f;
    }
    else if (dB < -60.0f)
    {
        def = (dB + 70.0f) * 0.25f;
    }
    else if (dB < -50.0f)
    {
        def = (dB + 60.0f) * 0.5f + 2.5f;
    }
    else if (dB < -40.0f)
    {
        def = (dB + 50.0f) * 0.75f + 7.5;
    }
    else if (dB < -30.0f)
    {
        def = (dB + 40.0f) * 1.5f + 15.0f;
    }
    else if (dB < -20.0f)
    {
        def = (dB + 30.0f) * 2.0f + 30.0f;
    }
    else if (dB < 0.0f)
    {
        def = (dB + 20.0f) * 2.5f + 50.0f;
    }
    else
    {
        //def = 100.0f;
    }

    return (int)((def / 100.0f) * ((float)size));
}
