#include "Biquad.h"

#include <cmath>

bool Biquad::reset(double sampleRate)
{
    _sampleRate = sampleRate;
    _z1 = 0.0;
    _z2 = 0.0;
    return true;
}

double Biquad::processSample(double xn)
{
    // Transposed DFII
    double yn = _coef.b0 * xn + _z1;
    _z1 = _coef.b1 * xn - _coef.a1 * yn + _z2;
    _z2 = _coef.b2 * xn - _coef.a2 * yn;

    return yn;
}

void Biquad::setParameters(BiquadParameters params)
{
    _params = params;
    _coef = buildCoefficients(_params);
}

BiquadParameters Biquad::getParameters()
{
    return _params;
}

BiquadCoefficients Biquad::buildCoefficients(BiquadParameters params)
{
    BiquadCoefficients result;

    double a0, a1, a2;
    double b0, b1, b2;

    double A = pow(10.0, params.gainDb / 40.0);
    double sqrtA = sqrt(A);
    double w0 = 2.0 * M_PI * params.frequency / _sampleRate;
    double sw0 = sin(w0);
    double cw0 = cos(w0);
    double alpha = sw0 / (2.0 * params.q);

    // from RBJ EQ Cookbook
    switch(params.type)
    {
        case BiquadFilterType::kLowpass:
            b0 = (1.0 - cw0) / 2.0;
            b1 = 1.0 - cw0;
            b2 = (1.0 - cw0) / 2.0;
            a0 = 1.0 + alpha;
            a1 = -2.0 * cw0;
            a2 = 1.0 - alpha;
            break;
        case BiquadFilterType::kHighpass:
            b0 = (1.0 + cw0) / 2.0;
            b1 = -(1.0 + cw0);
            b2 = (1.0 + cw0) / 2.0;
            a0 = 1.0 + alpha;
            a1 = -2.0 * cw0;
            a2 = 1.0 - alpha;
            break;
        case BiquadFilterType::kBandpass1:
            // Constant skirt gain
            b0 = sw0 / 2.0;
            b1 = 0.0;
            b2 = -sw0 / 2.0;
            a0 = 1.0 + alpha;
            a1 = -2.0 * cw0;
            a2 = 1.0 - alpha;
            break;
        case BiquadFilterType::kBandpass2:
            // Constant peak at 0dB
            b0 = alpha;
            b1 = 0.0;
            b2 = -alpha;
            a0 = 1.0 + alpha;
            a1 = -2.0 * cw0;
            a2 = 1.0 - alpha;
            break;
        case BiquadFilterType::kNotch:
            b0 = 1.0;
            b1 = -2.0 * cw0;
            b2 = 1.0;
            a0 = 1.0 + alpha;
            a1 = -2.0 * cw0;
            a2 = 1.0 - alpha;
            break;
        case BiquadFilterType::kAllpass:
            b0 = 1.0 - alpha;
            b1 = -2.0 * cw0;
            b2 = 1.0 + alpha;
            a0 = 1.0 + alpha;
            a1 = -2.0 * cw0;
            a2 = 1.0 - alpha;
            break;
        case BiquadFilterType::kPeak:
            b0 = 1.0 + alpha * A;
            b1 = -2.0 * cw0;
            b2 = 1.0 - alpha * A;
            a0 = 1.0 + alpha / A;
            a1 = -2.0 * cw0;
            a2 = 1.0 - alpha / A;
            break;
        case BiquadFilterType::kLowShelf:
            b0 = A * ((A + 1.0) - (A - 1.0) * cw0 + 2.0 * sqrtA * alpha);
            b1 = 2.0 * A * ((A - 1.0) - (A + 1.0) * cw0);
            b2 = A * ((A + 1.0) - (A - 1.0) * cw0 - 2.0 * sqrtA * alpha);
            a0 = (A + 1.0) + (A - 1.0) * cw0 + 2.0 * sqrtA * alpha;
            a1 = -2.0 * ((A - 1.0) + (A + 1.0) * cw0);
            a2 = (A + 1.0) + (A - 1.0) * cw0 - 2.0 * sqrtA * alpha;
            break;
        case BiquadFilterType::kHighShelf:
            b0 = A * ((A + 1.0) + (A - 1.0) * cw0 + 2.0 * sqrtA * alpha);
            b1 = -2.0 * A * ((A - 1.0) + (A + 1.0) * cw0);
            b2 = A * ((A + 1.0) + (A - 1.0) * cw0 - 2.0 * sqrtA * alpha);
            a0 = (A + 1.0) - (A - 1.0) * cw0 + 2.0 * sqrtA * alpha;
            a1 = 2.0 * ((A - 1.0) - (A + 1.0) * cw0);
            a2 = (A + 1.0) - (A - 1.0) * cw0 - 2.0 * sqrtA * alpha;
            break;
        
        default:
            a0 = 1.0;
            break;
    }

    result.a1 = a1 / a0;
    result.a2 = a2 / a0;
    result.b0 = b0 / a0;
    result.b1 = b1 / a0;
    result.b2 = b2 / a0;
    return result;
}

BiquadCoefficients Biquad::getCoefficients()
{
    return _coef;
}

double BiquadCoefficients::getMagnitudeAtFrequency(double f)
{
    auto comp = getComplexAmplitudeAtFrequency(f);
    double r = comp.real();
    double i = comp.imag();
    return sqrt(r * r + i * i);
}

double BiquadCoefficients::getPhaseAtFrequency(double f)
{   
    auto comp = getComplexAmplitudeAtFrequency(f);
    return atan2(comp.imag(), comp.real());
}

std::complex<double> BiquadCoefficients::getComplexAmplitudeAtFrequency(double f)
{
    /*
        Biquad transfer function

                b0 + b1*z^-1 + b2*z^-2
        H(z) = ------------------------
                a0 + a1*z^-1 + a2*z^-2
    */

    double w0 = 2.0 * M_PI * f;
    std::complex<double> zm1 = std::polar(1.0, -w0);
    std::complex<double> zm2 = std::polar(1.0, -2.0 * w0);

    std::complex<double> num = b0 + b1 * zm1 + b2 * zm2;
    std::complex<double> den = 1.0 + a1 * zm1 + a2 * zm2;
    return num / den;
}
