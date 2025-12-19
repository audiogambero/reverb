#pragma once

#include <JuceHeader.h>

class EQGraph : public Component
{
public:
    EQGraph();
    ~EQGraph();

    void paint(Graphics& g) override;
    void resized() override;
private:
    Image mBackground;
    void drawBackground();
    NormalisableRange<float> mRange;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQGraph)
};