#pragma once
#include "IAudioSignalProcessor.h"
#include "CircularBuffer.h"

/*
    Simple delay block

                  --------------
    x[n] >--------|    z^-d    |--------> y[n]
                  --------------
*/

struct DelayParameters
{
    double delayTime_mSec = 0.0;
    bool interpolate = false;
    double delay_Samples = 0.0;
};

class Delay : public IAudioSignalProcessor
{
public:
    bool reset(double sampleRate) override;
    void createBuffer(double sampleRate, double bufferLengthMs);
    
    double processSample(double in) override;
    void writeDelay(double in);
    double readDelay();
    double readDelayAtTime(double delayMs);
    double readDelayAtPercentage(double delayPercent);

    DelayParameters getParameters();
    void setParameters(DelayParameters params);
private:
    DelayParameters _params;
    CircularBuffer<double> _delayBuffer;
    double _samplesPerMs;
    double _bufferLengthMs;
    double _bufferLength;
    double _sampleRate;
};