#pragma once
#include "IAudioSignalProcessor.h"
#include "Delay.h"
#include "OnePoleLPF.h"
#include "LFO.h"

struct DelayAPFParameters
{
    bool interpolate = true;
    double delayTimeMs = 0.0;
    double apfG = 0.0;
    
    bool enableLpf = false;
    double lpfG = 0.0;

    bool enableLfo = false;
    double lfoRate = 0.0;
    double lfoAmount = 0.0;
    double maxLfoModMs = 0.0;
};

class DelayAPF : public IAudioSignalProcessor
{
public:
    bool reset(double sampleRate) override;
    double processSample(double xn) override;
    
    DelayAPFParameters getParameters();
    void setParameters(DelayAPFParameters params);

    void createBuffer(double sampleRate, double bufferLengthMs);
private:
    DelayAPFParameters _params;

    Delay _delay;
    LFO _modLfo;
    OnePoleLPF _lpf;

    double _sampleRate;
    double _bufferLengthMs;

};