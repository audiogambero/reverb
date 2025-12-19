#pragma once

#include "IAudioSignalProcessor.h"
#include "Biquad.h"

struct ShelfPairParameters
{
    double lowF0;
    double lowQ;
    double lowGainDb;
    double highF0;
    double highQ;
    double highGainDb;
};

class ShelfPair : public IAudioSignalProcessor
{
public:
    bool reset(double sampleRate) override;
    double processSample(double xn) override;
    
    void setParameters(ShelfPairParameters params);
    ShelfPairParameters getParameters();

    double getMagnitudeAtFrequency(double f);
    double getPhaseAtFrequency(double f);
    std::complex<double> getComplexAmplitudeAtFrequency(double f);
private:
    double _sampleRate;
    ShelfPairParameters _params;

    Biquad _lowShelf;
    Biquad _highShelf;
};