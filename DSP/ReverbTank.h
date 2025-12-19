#pragma once
#include "IAudioSignalProcessor.h"
#include "DelayAPF.h"
#include "NestedDelayAPF.h"
#include "ShelfPair.h"

/*
    Reverb tank
    Input diffusion akin to Datorro's reverb topology
    Allpass loop reverb tail based on Keith Barr topology
*/

struct ReverbTankParameters
{
    double maxDelayMs = 100.0;
    double maxApfMs = 100.0;
    double weightDelay = 0.0;
    double weightApf = 0.0;
    double preDelayMs = 0.0;
    double feedback = 0.7;
    double lpfG = 0.0;
    double modAmount = 1.0;
    double mix = 0.0;
    bool diffuseInput = false;

    double tailShelfLowF;
    double tailShelfLowGain;
    double tailShelfHighF;
    double tailShelfHighGain;
};

#define NUM_CHANNELS 2
#define NUM_BRANCHES 4
#define NUM_DIFF_STEPS 4

class ReverbTank : public IAudioSignalProcessor
{
public:
    bool reset(double sampleRate) override;
    double processSample(double xn) override;
    bool canProcessAudioFrame() override { return true; }
    bool processAudioFrame(const float* inputFrame, float* outputFrame, 
        uint32_t inputChannels, uint32_t outputChannels) override;

    ReverbTankParameters getParameters();
    void setParameters(ReverbTankParameters params);
private:
    ReverbTankParameters _params;
    double _sampleRate;
    
    Delay _preDelay;
    Delay _branchDelays[NUM_BRANCHES];
    DelayAPF _diffusionApf[NUM_DIFF_STEPS];
    NestedDelayAPF _branchApf[NUM_BRANCHES];
    ShelfPair _branchShelf[NUM_BRANCHES];

    // Input diffusion
    double _diffApfGains[NUM_DIFF_STEPS] = { 0.75, 0.75, 0.625, 0.625 };
    double _diffApfTimes[NUM_DIFF_STEPS] = { 4.762, 3.582, 12.721, 9.297 }; //mS

    double _lfoRates[NUM_BRANCHES] = { 0.15, 0.33, 0.57, 0.73 };            //Hz
    // double _apfTimeWeights[NUM_BRANCHES] = { 0.317, 0.873, 0.477, 0.291 };

    // Nested allpass delays (inner and outer delay times)
    double _apfTimeWeights[NUM_BRANCHES * 2] = { 0.317, 0.873, 0.477, 0.291,
                                                 0.993, 0.757, 0.179, 0.575 };
    double _delayTimeWeights[NUM_BRANCHES] = { 1.0, 0.873, 0.707, 0.667 };
};
