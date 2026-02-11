#pragma once

#include "../../PluginProcessor.h"
#include "../../DSP/ShelfPair.h"

class EQGraph : public Component, public AudioProcessorValueTreeState::Listener
{
public:
    EQGraph(AudioPluginAudioProcessor& p);
    ~EQGraph();

    void paint(Graphics& g) override;
    void resized() override;

    void parameterChanged(const String& parameterId, float newValue) override;
private:
    AudioPluginAudioProcessor& rProc;
    AudioProcessorValueTreeState& rValueTreeState;
    Image mBackground;
    void drawBackground();
    NormalisableRange<float> mRangeDb;
    NormalisableRange<float> mRangeFreq;

    const int GRAPH_RESOLUTION = 32;

    Path mPath;
    ShelfPair mFilters;

    float dbToY(double db);
    float fToX(double f);
    float xToF(double x);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQGraph)
};