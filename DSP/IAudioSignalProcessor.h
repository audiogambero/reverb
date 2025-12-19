#pragma once
#include <stdint.h>

class IAudioSignalProcessor
{
    virtual bool reset(double sampleRate) = 0;

    virtual double processSample(double xn) = 0;
    virtual bool canProcessAudioFrame() { return false; }
    virtual bool processAudioFrame(const float* inputFrame, float* ouptutFrame, 
        uint32_t inputChannels, uint32_t outputChannels) { return false; }

    virtual void enableAuxInput(bool enable) {}
    virtual double processAuxInputAudioSample(double xn) { return xn; }
};