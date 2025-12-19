#include "Delay.h"

bool Delay::reset(double sampleRate)
{
    _sampleRate = sampleRate;
    return true;
}

void Delay::createBuffer(double sampleRate, double bufferLengthMs)
{
    _sampleRate = sampleRate;
    _bufferLengthMs = bufferLengthMs;
    _samplesPerMs = _sampleRate / 1000.0;

    _bufferLength = _bufferLengthMs * _samplesPerMs;
    _delayBuffer.create((unsigned int)_bufferLength);
}

// =================================================================

double Delay::processSample(double in)
{
    writeDelay(in);
    return readDelay();
}

void Delay::writeDelay(double in)
{
    _delayBuffer.write(in);
}

double Delay::readDelay()
{
    double delaySamples = _params.delayTime_mSec * _samplesPerMs;
    return _delayBuffer.read(delaySamples, _params.interpolate);
}

double Delay::readDelayAtTime(double delayMs)
{
    double delaySamples = delayMs * _samplesPerMs;
    return _delayBuffer.read(delaySamples, _params.interpolate);
}

double Delay::readDelayAtPercentage(double delayPercent)
{
    double delaySamples = delayPercent / 100.0 * _params.delayTime_mSec * _samplesPerMs;
    return _delayBuffer.read(delaySamples, _params.interpolate);
}

// =================================================================

DelayParameters Delay::getParameters()
{
    return _params;
}

void Delay::setParameters(DelayParameters params)
{
    _params = params;
}