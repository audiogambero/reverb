#pragma once
#include "IAudioSignalGenerator.h"

struct LFOParameters
{
    GeneratorWaveform waveform;
    double frequencyHz;
};

class LFO : public IAudioSignalGenerator
{
public:
    bool reset(double sampleRate) override;
    void setGeneratorFrequency(double freq) override;
    void setGeneratorWaveform(GeneratorWaveform waveform) override;
    const SignalGenData renderAudioOutput() override;

    LFOParameters getParameters();
    void setParameters(LFOParameters params);

    void setSampleRate(double sampleRate);
    bool checkAndWrapModulo(double& moduloCounter, double phaseInc);
    bool advanceAndCheckAndWrapModulo(double& moduloCounter, double phaseInc);
    void advanceModulo(double& moduloCounter, double phaseInc);

private:
    LFOParameters _params;

    double _sampleRate;
    double _modCounter;
    double _modCounterQP;
    double _phaseIncrement;

    double parabolicSin(double angle);

    inline double unipolarToBipolar(double in)
    {
        return in * 2.0 - 1.0;
    }
};