#pragma once

#include <JuceHeader.h>

class NeverbLookAndFeel : public LookAndFeel_V4
{
public:
    NeverbLookAndFeel();
    ~NeverbLookAndFeel();

    Font getDefaultLabelFont();
    Font getDefaultValueFont();
    Font getDefaultUIFont();
private:
    juce::Typeface::Ptr mTypeface;
};