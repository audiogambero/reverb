#pragma once

#include "IAudioSignalProcessor.h"
#include "DelayAPF.h"

struct NestedDelayAPFParams
{
    DelayAPFParameters innerApfParams;
    double outerApfDelay;
    double outerApfG;
};

class NestedDelayAPF : public IAudioSignalProcessor
{
public:
    bool reset(double sampleRate) override;
    double processSample(double xn) override;

    void createBuffer(double sampleRate, double bufferTimeMs);

    void setParameters(NestedDelayAPFParams params);
    NestedDelayAPFParams getParameters();
private:
    double _sampleRate;
    double _bufferSizeMs;
    NestedDelayAPFParams _params;

    DelayAPF _innerApf;
    Delay _delay;
};