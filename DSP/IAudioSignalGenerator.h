#pragma once

enum class GeneratorWaveform
{
    kTriangle,
    kSin,
    kSaw,
    kWhiteNoise,
    kRandomSampleAndHold
};

struct SignalGenData
{
    SignalGenData() {}
    double normalOutput = 0.0;
    double invertedOutput = 0.0;
    double quadPhaseOutputPos = 0.0;
    double quadPhaseOutputNeg = 0.0;
};

class IAudioSignalGenerator
{
    virtual bool reset(double sampleRate) = 0;
    virtual void setGeneratorFrequency(double freq) = 0;
    virtual void setGeneratorWaveform(GeneratorWaveform waveform) = 0;
    virtual const SignalGenData renderAudioOutput() = 0;
};