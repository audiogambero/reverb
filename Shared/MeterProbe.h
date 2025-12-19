#pragma once
#include <cmath>

class MeterProbe
{
public:
    void reset()
    {
        currentMax = 0.0;
    }

    void pushSample(double xn) 
    {
        double xabs = abs(xn);
        if(xabs > currentMax)
            currentMax = xabs;
    }

    double getPeak(bool clear)
    {
        double out = currentMax;
        if(clear)
            reset();
        return out;
    }
private:
    double currentMax = 0.0;
};