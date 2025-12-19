#pragma once

#include <complex>
#include "IAudioSignalProcessor.h"

enum class BiquadFilterType
{
    kLowpass,
    kHighpass,
    kBandpass1,     //Constant skirt gain
    kBandpass2,     //Constant 0dB peak
    kNotch,
    kAllpass,
    kPeak,
    kLowShelf,
    kHighShelf
};

struct BiquadParameters
{
    double frequency;
    double q;
    double gainDb;

    BiquadFilterType type;
};

struct BiquadCoefficients
{
    double a1, a2;
    double b0, b1, b2;

    double getMagnitudeAtFrequency(double f);
    double getPhaseAtFrequency(double f);
    std::complex<double> getComplexAmplitudeAtFrequency(double f);
};

class Biquad : IAudioSignalProcessor
{
public:
    bool reset(double sampleRate) override;
    double processSample(double xn) override;

    void setParameters(BiquadParameters params);
    BiquadParameters getParameters();

    BiquadCoefficients getCoefficients();
private:
    double _sampleRate;

    BiquadParameters _params;
    BiquadCoefficients _coef;

    double _z1;
    double _z2;

    BiquadCoefficients buildCoefficients(BiquadParameters params);
};