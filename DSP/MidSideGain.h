#pragma once
#include "IAudioSignalProcessor.h"

struct MidSideGainParameters
{
    double midGainDb;
    double sideGainDb;
};

class MidSideGain : public IAudioSignalProcessor
{
public:
    bool reset(double sampleRate) override;
    double processSample(double xn) override;
    bool canProcessAudioFrame() { return true; }
    bool processAudioFrame(const float* inputFrame, float* outputFrame, uint32_t inputChannels, uint32_t outputChannels) override;

    void setParameters(MidSideGainParameters params);
    MidSideGainParameters getParameters();
private:
    MidSideGainParameters _params;
    double _midGain;
    double _sideGain;
};