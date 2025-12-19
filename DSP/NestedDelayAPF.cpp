#include "NestedDelayAPF.h"

bool NestedDelayAPF::reset(double sampleRate)
{
    _sampleRate = sampleRate;

    _innerApf.reset(_sampleRate);
    _delay.reset(_sampleRate);

    return true;
}

double NestedDelayAPF::processSample(double xn)
{
    double delayed = _delay.readDelay();
    double sum1 = xn + delayed * _params.outerApfG;
    _delay.writeDelay(_innerApf.processSample(sum1));
    return delayed - sum1 * _params.outerApfG;
}

void NestedDelayAPF::createBuffer(double sampleRate, double bufferLengthMs)
{
    _delay.createBuffer(sampleRate, bufferLengthMs);
    _innerApf.createBuffer(sampleRate, bufferLengthMs);
    _bufferSizeMs = bufferLengthMs;
}

void NestedDelayAPF::setParameters(NestedDelayAPFParams params)
{
    _params = params;
    _innerApf.setParameters(_params.innerApfParams);
    
    auto delayParams = _delay.getParameters();
    delayParams.delayTime_mSec = _params.outerApfDelay;
    delayParams.interpolate = false;    // no need for interpolation, static
    _delay.setParameters(delayParams);
}

NestedDelayAPFParams NestedDelayAPF::getParameters()
{
    return _params;
}