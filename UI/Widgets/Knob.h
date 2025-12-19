#pragma once

#include <JuceHeader.h>

class Knob : public Slider
{
public:
    Knob();
    ~Knob() override;

    void paint(Graphics& g) override;
    void resized() override;
private:
    Colour outerColour1, outerColour2, innerColour1, innerColour2, tickColour;
    Rectangle<float> knobArea;

    Rectangle<float> shrinkByPercentage(Rectangle<float> area, float amt, bool keepCenter);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Knob)
};