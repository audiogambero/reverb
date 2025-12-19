#include "ReverbTank.h"

bool ReverbTank::reset(double sampleRate)
{
    _sampleRate = sampleRate;

    _preDelay.reset(_sampleRate);
    _preDelay.createBuffer(_sampleRate, 250.0);

    for(int s = 0; s < NUM_DIFF_STEPS; s++)
    {
        _diffusionApf[s].reset(_sampleRate);
        _diffusionApf[s].createBuffer(_sampleRate, 10.0);
    }

    for(int b = 0; b < NUM_BRANCHES; b++)
    {
        _branchDelays[b].reset(_sampleRate);
        _branchDelays[b].createBuffer(_sampleRate, 100.0);

        _branchApf[b].reset(_sampleRate);
        _branchApf[b].createBuffer(_sampleRate, 100.0);

        _branchShelf[b].reset(_sampleRate);
    }
    return true;
}

double ReverbTank::processSample(double xn)
{
    return xn;
}

void ReverbTank::setParameters(ReverbTankParameters params)
{
    _params = params;

    auto predelayParams = _preDelay.getParameters();
    predelayParams.delayTime_mSec = _params.preDelayMs;
    _preDelay.setParameters(predelayParams);

    auto diffApfParams = _diffusionApf[0].getParameters();
    diffApfParams.interpolate = true;
    for(int s = 0; s < NUM_DIFF_STEPS; s++)
    {
        diffApfParams.delayTimeMs = _diffApfTimes[s];
        diffApfParams.apfG = _diffApfGains[s];
        _diffusionApf[s].setParameters(diffApfParams);
    }

    auto apfParams = _branchApf[0].getParameters();
    auto innerApfParams = apfParams.innerApfParams;
    innerApfParams.apfG = 0.6;
    innerApfParams.enableLpf = true;
    innerApfParams.lpfG = _params.lpfG;
    innerApfParams.interpolate = true;
    innerApfParams.enableLfo = true;
    innerApfParams.maxLfoModMs = 3.0;
    apfParams.outerApfG = 0.5;

    auto shelfParams = _branchShelf[0].getParameters();
    shelfParams.lowQ = 0.707;
    shelfParams.highQ = 0.707;
    shelfParams.lowF0 = _params.tailShelfLowF;
    shelfParams.lowGainDb = _params.tailShelfLowGain;
    shelfParams.highF0 = _params.tailShelfHighF;
    shelfParams.highGainDb = _params.tailShelfHighGain;

    auto delayParams = _branchDelays[0].getParameters();
    for(int i = 0; i < NUM_BRANCHES; i++)
    {
        innerApfParams.delayTimeMs = _apfTimeWeights[i * 2 + 1] * _params.maxApfMs * _params.weightApf;
        apfParams.outerApfDelay = _apfTimeWeights[i * 2] * _params.maxApfMs * _params.weightApf;
        innerApfParams.lfoRate = _lfoRates[i];
        innerApfParams.lfoAmount = _params.modAmount;
        apfParams.innerApfParams = innerApfParams;
        _branchApf[i].setParameters(apfParams);

        delayParams.delayTime_mSec = _delayTimeWeights[i] * _params.maxDelayMs * _params.weightDelay;
        _branchDelays[i].setParameters(delayParams);

        _branchShelf[i].setParameters(shelfParams);
    }
}

ReverbTankParameters ReverbTank::getParameters()
{
    return _params;
}

bool ReverbTank::processAudioFrame(const float* inputFrame, float* outputFrame, uint32_t inputChannels, uint32_t outputChannels)
{
    double globalFb = _branchDelays[NUM_BRANCHES - 1].readDelay();
    double fb = globalFb * _params.feedback;

    //Sum input to mono
    double xnL = inputFrame[0];
    double xnR = inputChannels > 1 ? inputFrame[1] : 0.0;
    double gain = 1.0 / double(inputChannels);
    double xnMono = xnL * gain + xnR * gain;

    double inputDiff = _preDelay.processSample(xnMono);
    if(_preDelay.getParameters().delayTime_mSec == 0)
        inputDiff = xnMono;
    
    if(_params.diffuseInput)
    {
        for(int s = 0; s < NUM_DIFF_STEPS; s++)
        {
            inputDiff = _diffusionApf[s].processSample(inputDiff);
        }
    }

    double input = inputDiff + fb;

    for(int i = 0; i < NUM_BRANCHES; i++)
    {
        double apfOut = _branchApf[i].processSample(input);
        double shelfOut = _branchShelf[i].processSample(apfOut);
        double delayOut = _branchDelays[i].processSample(shelfOut);
        input = delayOut + inputDiff;
    }

    double w = 0.707 / 4.0;
    double outL = 0.0;
    outL += w * _branchDelays[0].readDelayAtPercentage(23.0);
    outL -= w * _branchDelays[1].readDelayAtPercentage(43.0);
    outL += w * _branchDelays[2].readDelayAtPercentage(59.0);
    outL -= w * _branchDelays[3].readDelayAtPercentage(79.0);

    outL += w * _branchDelays[0].readDelayAtPercentage(31.0);
    outL -= w * _branchDelays[1].readDelayAtPercentage(47.0);
    outL += w * _branchDelays[2].readDelayAtPercentage(67.0);
    outL -= w * _branchDelays[3].readDelayAtPercentage(83.0);

    double outR = 0.0;
    outR += w * _branchDelays[0].readDelayAtPercentage(29.0);
    outR -= w * _branchDelays[1].readDelayAtPercentage(41.0);
    outR += w * _branchDelays[2].readDelayAtPercentage(61.0);
    outR -= w * _branchDelays[3].readDelayAtPercentage(73.0);

    outR += w * _branchDelays[0].readDelayAtPercentage(37.0);
    outR -= w * _branchDelays[1].readDelayAtPercentage(53.0);
    outR += w * _branchDelays[2].readDelayAtPercentage(71.0);
    outR -= w * _branchDelays[3].readDelayAtPercentage(89.0);

    if(outputChannels == 1)
        outputFrame[0] = (0.5f * (float)outL + 0.5f * (float)outR) * (float)_params.mix + (float)xnMono * (float)(1.0 - _params.mix);
    else
    {
        outputFrame[0] = (float)outL * (float)_params.mix + (float)xnL * (float)(1.0 - _params.mix);
        outputFrame[1] = (float)outR * (float)_params.mix + (float)xnR * (float)(1.0 - _params.mix);
    }
    return true;
}