#include "NeverbLookAndFeel.h"

NeverbLookAndFeel::NeverbLookAndFeel()
{
    mTypeface = Typeface::createSystemTypefaceFor(BinaryData::InterVariable_ttf, BinaryData::InterVariable_ttfSize);
    setDefaultSansSerifTypeface(mTypeface);

    setColour(Label::textColourId, Colour(0xFF1C2422));

    setDefaultLookAndFeel(this);
}

NeverbLookAndFeel::~NeverbLookAndFeel()
{

}

Font NeverbLookAndFeel::getDefaultLabelFont()
{
    auto fo = juce::FontOptions(mTypeface);
    return Font(fo.withHeight(18.0f).withStyle("Bold"));
}

Font NeverbLookAndFeel::getDefaultValueFont()
{
    auto fo = juce::FontOptions(mTypeface);
    return Font(fo.withHeight(10.0f).withStyle("Normal"));
}

Font NeverbLookAndFeel::getDefaultUIFont()
{
    auto fo = juce::FontOptions(mTypeface);
    return Font(fo.withHeight(12.0f).withStyle("Medium"));
}