#include "OnePoleLPF.h"

OnePoleLPFParameters OnePoleLPF::getParameters()
{
    return _params;
}

void OnePoleLPF::setParameters(OnePoleLPFParameters params)
{
    _params = params;
}

bool OnePoleLPF::reset(double sampleRate)
{
    _z1 = 0.0;
    return true;
}

double OnePoleLPF::processSample(double xn)
{
    double g = _params.g;
    double out = xn * (1.0 - g) + _z1 * g;
    _z1 = out;
    return out;
}