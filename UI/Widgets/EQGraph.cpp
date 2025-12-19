#include "EQGraph.h"

EQGraph::EQGraph()
{
    mRange = NormalisableRange<float>(-18.0, 6.0, 0.1f);
}

EQGraph::~EQGraph()
{

}

void EQGraph::paint(Graphics& g)
{
    g.drawImageAt(mBackground, 0, 0);
}

void EQGraph::resized()
{
    mBackground = Image(Image::PixelFormat::ARGB, getWidth(), getHeight(), true);
    drawBackground();
}

void EQGraph::drawBackground()
{
    Graphics g(mBackground);
    float cntX = (float)getWidth() / 2.f;
    ColourGradient bgFill(Colour(0xFF1C2422), cntX, 0.f, Colour(0xFF080A09), cntX, (float)getHeight(), false);
    g.setGradientFill(bgFill);
    g.fillRoundedRectangle(mBackground.getBounds().toFloat(), 3.f);

    float w = mBackground.getWidth();
    float h = mBackground.getHeight();

    g.setColour(Colour(0x33FFFFFF));
    float db0y = jmap(mRange.convertTo0to1(0.f), h, 0.f);
    g.drawLine(0.f, db0y, w, db0y);
}