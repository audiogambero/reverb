#include "Knob.h"

Knob::Knob() : Slider(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextBoxBelow)
{
    outerColour1 = Colour(0xFF004455);
    outerColour2 = Colour(0xFF002A34);
    innerColour1 = Colour(0xFF002F3C);
    innerColour2 = Colour(0xFF00576D);
    tickColour = Colour(0xFFF6FBFB);
}

Knob::~Knob()
{

}

void Knob::paint(Graphics& g)
{
    Point<float> cnt = getLocalBounds().toFloat().getCentre();
    Point<float> top = cnt.withY(0);
    Point<float> btm = cnt.withY(getHeight());

    ColourGradient outerFill(outerColour1, top, outerColour2, btm, false);
    ColourGradient innerFill(innerColour1, top, innerColour2, btm, false);
    ColourGradient tickFill(tickColour.interpolatedWith(Colours::black, 0.6), cnt, tickColour, top, true);

    g.setGradientFill(outerFill);
    g.fillEllipse(knobArea);

    g.setGradientFill(innerFill);
    g.fillEllipse(shrinkByPercentage(knobArea, 25.f, true));

    // Define tick parameters based on knob size
    float knobDiameter = knobArea.getWidth();
    float tickWidth = knobDiameter * 0.075f; // 5% of knob diameter
    float tickHeight = knobDiameter * 0.12f; // 40% of knob diameter

    Rectangle<float> tickRect = Rectangle<float>(
        cnt.x - tickWidth / 2.0f,
        0.f,
        tickWidth,
        tickHeight
    );
    float prc = (getValue() - getMinimum()) / (getMaximum() - getMinimum());
    float rotationAngle = degreesToRadians((prc * 270.0f) - 135.0f);
    AffineTransform rotationTransform = AffineTransform::rotation(rotationAngle, cnt.x, cnt.y);
    
    Graphics::ScopedSaveState state(g);
    g.addTransform(rotationTransform);

    g.setGradientFill(tickFill);
    g.fillRect(tickRect);
}

void Knob::resized()
{
    knobArea = shrinkByPercentage(Rectangle<float>(0, 0, getWidth(), getWidth()), 0.f, true);
}

Rectangle<float> Knob::shrinkByPercentage(Rectangle<float> area, float amt, bool keepCenter)
{
    float mult = 1.0 - amt / 100.f;
    float w = area.getWidth() * mult;
    float h = area.getHeight() * mult;
    if(keepCenter)
        return area.withSizeKeepingCentre(w, h);
    return area.withSize(w, h);
}