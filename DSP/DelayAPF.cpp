#include "DelayAPF.h"

bool DelayAPF::reset(double sampleRate)
{
    _sampleRate = sampleRate;
    _delay.reset(_sampleRate);
    _modLfo.reset(_sampleRate);
    _lpf.reset(_sampleRate);
    return true;
}

double DelayAPF::processSample(double xn)
{
    double lfoValMs = _modLfo.renderAudioOutput().normalOutput * _params.lfoAmount * _params.maxLfoModMs;
    double delayed = _delay.readDelayAtTime(_params.delayTimeMs + lfoValMs);
    double sum1 = xn + delayed * _params.apfG;
    if(_params.enableLpf)
        _delay.writeDelay(_lpf.processSample(sum1));
    else
        _delay.writeDelay(sum1);
    double sum2 = delayed - sum1 * _params.apfG;
    return sum2;
}

DelayAPFParameters DelayAPF::getParameters()
{
    return _params;
}

void DelayAPF::setParameters(DelayAPFParameters params)
{
    _params = params;

    auto dlyParams = _delay.getParameters();
    dlyParams.delayTime_mSec = _params.delayTimeMs;
    dlyParams.interpolate = _params.interpolate;
    _delay.setParameters(dlyParams);

    auto lfoParams = _modLfo.getParameters();
    lfoParams.frequencyHz = _params.lfoRate;
    lfoParams.waveform = GeneratorWaveform::kSin;
    _modLfo.setParameters(lfoParams);

    auto lpfParams = _lpf.getParameters();
    lpfParams.g = _params.lpfG;
    _lpf.setParameters(lpfParams);
}

void DelayAPF::createBuffer(double sampleRate, double bufferLengthMs)
{
    _sampleRate = sampleRate;
    _bufferLengthMs = bufferLengthMs;
    _delay.createBuffer(_sampleRate, _bufferLengthMs);
}