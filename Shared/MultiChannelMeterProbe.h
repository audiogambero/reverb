#pragma once
#include <JuceHeader.h>
#include "MeterProbe.h"

class MultiChannelMeterProbe
{
public:
    void setNumChannels(unsigned int ch)
    {
        mProbes.resize(ch);
    }

    unsigned int getNumChannels()
    {
        return mProbes.size();
    }

    void pushSample(double xn, unsigned int ch)
    {
        mProbes[ch].pushSample(xn);
    }

    void pushFrame(const float* inFrame, unsigned int numChannels)
    {
        for(int c = 0; c < numChannels; c++)
        {
            mProbes[c].pushSample(inFrame[c]);
        }
    }

    MeterProbe& getProbe(unsigned int ch)
    {
        return mProbes[ch];
    }

private:
    std::vector<MeterProbe> mProbes;
};
