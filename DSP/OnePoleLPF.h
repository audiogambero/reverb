#pragma once

#include "IAudioSignalProcessor.h"

/*
    Simple single pole lowpass filter

    x[n] ---(1-g)--->+-----------------------> y[n]
                      |                   |
                    (g)                  |
                    |--------z^-1-------|
*/

struct OnePoleLPFParameters
{
    double g = 0.0;
};

class OnePoleLPF : IAudioSignalProcessor
{
public:
    bool reset(double sampleRate) override;
    double processSample(double xn) override;

    OnePoleLPFParameters getParameters();
    void setParameters(OnePoleLPFParameters params);
private:
    OnePoleLPFParameters _params;
    double _z1;
};