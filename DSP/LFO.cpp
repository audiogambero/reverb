#include "LFO.h"

#include <cmath>


bool LFO::reset(double sampleRate)
{
    _sampleRate = sampleRate;

    _modCounter = 0.0;
    _modCounterQP = 0.25;
    return true;
}

void LFO::setGeneratorFrequency(double freq)
{
    _params.frequencyHz = freq;
    _phaseIncrement = _params.frequencyHz / _sampleRate;
}

void LFO::setGeneratorWaveform(GeneratorWaveform waveform)
{
    _params.waveform = waveform;
}

const SignalGenData LFO::renderAudioOutput()
{
    checkAndWrapModulo(_modCounter, _phaseIncrement);
    _modCounterQP = _modCounter;

    advanceAndCheckAndWrapModulo(_modCounterQP, 0.25);

    SignalGenData output;
    
    if(_params.waveform == GeneratorWaveform::kSin)
    {
        double angle = _modCounter * 2.0 * M_PI - M_PI;
        output.normalOutput = parabolicSin(-angle);
        angle = _modCounterQP * 2.0 * M_PI - M_PI;
        output.quadPhaseOutputPos = parabolicSin(-angle);
    }

    if(_params.waveform == GeneratorWaveform::kSaw)
    {
        output.normalOutput = unipolarToBipolar(_modCounter);
        output.quadPhaseOutputPos = unipolarToBipolar(_modCounterQP);
    }

    if(_params.waveform == GeneratorWaveform::kTriangle)
    {
        output.normalOutput = unipolarToBipolar(_modCounter);
        output.normalOutput = 2.0 * fabs(output.normalOutput) - 1.0;

        output.quadPhaseOutputPos = unipolarToBipolar(_modCounterQP);
        output.quadPhaseOutputPos = 2.0 * fabs(output.quadPhaseOutputPos) - 1.0;
    }

    output.invertedOutput = -output.normalOutput;
    output.quadPhaseOutputNeg = -output.quadPhaseOutputPos;

    advanceModulo(_modCounter, _phaseIncrement);
    return output;
}

LFOParameters LFO::getParameters()
{
    return _params;
}

void LFO::setParameters(LFOParameters params)
{
    _params = params;
    setGeneratorFrequency(_params.frequencyHz);
    setGeneratorWaveform(_params.waveform);
}

void LFO::setSampleRate(double sampleRate)
{
    _sampleRate = sampleRate;
}

bool LFO::checkAndWrapModulo(double& moduloCounter, double phaseInc)
{
    bool wrap = moduloCounter >= 1.0;
    if(wrap)
        moduloCounter -= 1.0;
    return wrap;
}

bool LFO::advanceAndCheckAndWrapModulo(double& moduloCounter, double phaseInc)
{
    advanceModulo(moduloCounter, phaseInc);
    return checkAndWrapModulo(moduloCounter, phaseInc);
}

void LFO::advanceModulo(double& moduloCounter, double phaseInc)
{
    moduloCounter += phaseInc;
}

double LFO::parabolicSin(double x)
{
    const double B = 4.0 / M_PI;
    const double C = -4.0 / (M_PI * M_PI);
    const double P = 0.225;

    double y = B * x + C * x * fabs(x);
    y = P * (y * fabs(y) - y) + y;
    return y;
}