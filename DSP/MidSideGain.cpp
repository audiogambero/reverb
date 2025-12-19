#include "MidSideGain.h"

#include <cmath>

bool MidSideGain::reset(double sampleRate)
{
    return true;
}

double MidSideGain::processSample(double xn)
{
    // can't do MS processing on single channel!
    return xn;
}

bool MidSideGain::processAudioFrame(const float* inputFrame, float* outputFrame, uint32_t inputChannels, uint32_t outputChannels)
{
    if(inputChannels != 2 || outputChannels != 2)
        return false;
    
    double xnL = inputFrame[0];
    double xnR = inputFrame[1];

    double mid = xnL + xnR;
    double side = xnL - xnR;

    mid *= _midGain;
    side *= _sideGain;

    outputFrame[0] = (mid + side) / 2.0;
    outputFrame[1] = (mid - side) / 2.0;
    
    return true;
}

void MidSideGain::setParameters(MidSideGainParameters params)
{
    _params = params;
    _midGain = pow(10, _params.midGainDb / 20.0);
    _sideGain = pow(10, _params.sideGainDb / 20.0);
}

MidSideGainParameters MidSideGain::getParameters()
{
    return _params;
}