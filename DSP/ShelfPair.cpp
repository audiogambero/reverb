#include "ShelfPair.h"

bool ShelfPair::reset(double sampleRate)
{
    _sampleRate = sampleRate;
    _lowShelf.reset(_sampleRate);
    _highShelf.reset(_sampleRate);
    return true;
}

double ShelfPair::processSample(double xn)
{
    double yn = _lowShelf.processSample(xn);
    yn = _highShelf.processSample(yn);
    return yn;
}

void ShelfPair::setParameters(ShelfPairParameters params)
{
    _params = params;

    auto lowShelfParams = _lowShelf.getParameters();
    lowShelfParams.type = BiquadFilterType::kLowShelf;
    lowShelfParams.frequency = params.lowF0;
    lowShelfParams.q = params.lowQ;
    lowShelfParams.gainDb = params.lowGainDb;
    _lowShelf.setParameters(lowShelfParams);

    auto highShelfParams = _highShelf.getParameters();
    highShelfParams.type = BiquadFilterType::kHighShelf;
    highShelfParams.frequency = params.highF0;
    highShelfParams.q = params.highQ;
    highShelfParams.gainDb = params.highGainDb;
    _highShelf.setParameters(highShelfParams);
}

ShelfPairParameters ShelfPair::getParameters()
{
    return _params;
}

double ShelfPair::getMagnitudeAtFrequency(double f)
{
    auto comp = getComplexAmplitudeAtFrequency(f);
    double r = comp.real();
    double i = comp.imag();
    return sqrt(r * r + i * i);
}

double ShelfPair::getPhaseAtFrequency(double f)
{   
    auto comp = getComplexAmplitudeAtFrequency(f);
    return atan2(comp.imag(), comp.real());
}

std::complex<double> ShelfPair::getComplexAmplitudeAtFrequency(double f)
{
    auto compL = _lowShelf.getCoefficients().getComplexAmplitudeAtFrequency(f);
    auto compH = _highShelf.getCoefficients().getComplexAmplitudeAtFrequency(f);
    return compL * compH;
}
