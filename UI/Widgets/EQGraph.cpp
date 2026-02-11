#include "EQGraph.h"

EQGraph::EQGraph(AudioPluginAudioProcessor& p) : rProc(p), rValueTreeState(p.getValueTreeState())
{
    mRangeDb = NormalisableRange<float>(-24.0, 3.0, 0.1f);
    mRangeFreq = NormalisableRange<float>(50.0, 20e3f, 0.1f, 0.2f);

    auto& apvts = rProc.getValueTreeState();
    apvts.addParameterListener(Parameters::ID_TAIL_LOW_F, this);
    apvts.addParameterListener(Parameters::ID_TAIL_LOW_G, this);
    apvts.addParameterListener(Parameters::ID_TAIL_HIGH_F, this);
    apvts.addParameterListener(Parameters::ID_TAIL_HIGH_G, this);

    mFilters.reset(48000.0); // arbitrary sample rate, these filters are just for viz
}

EQGraph::~EQGraph()
{

}

void EQGraph::paint(Graphics& g)
{
    g.drawImageAt(mBackground, 0, 0);
    g.setColour(Colours::white);
    g.strokePath(mPath, PathStrokeType (1.f), {});
}

void EQGraph::resized()
{
    parameterChanged("", 0);
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
    const auto dbsGrid = { -6.f, -12.f, -18.f, -24.f};
    for(auto d : dbsGrid)
    {
        float y = dbToY(d);
        g.drawLine(0, y, w, y);
    }

    float db0y = dbToY(0.f);
    g.drawLine(0.f, db0y, w, db0y);

    const auto freqsMain = { 50.f, 100.f, 500.f, 1000.f, 5000.f, 10e3f, 15e3f};
    for(auto f : freqsMain)
    {
        float x = fToX(f);
        g.drawLine(x, 0, x, h);
    }
}

float EQGraph::dbToY(double db)
{
    return jmap(mRangeDb.convertTo0to1(db), (float)getHeight(), 0.f);
}

float EQGraph::fToX(double f)
{
    return jmap(mRangeFreq.convertTo0to1(f), 0.f, (float)getWidth());
}

float EQGraph::xToF(double x)
{
    double xNorm = x / (double)getWidth();
    return mRangeFreq.convertFrom0to1(xNorm);
}

void EQGraph::parameterChanged(const String& parameterId, float newValue)
{
    auto params = mFilters.getParameters();
    params.lowQ = 0.707;
    params.highQ = 0.707;
    params.lowF0 = rValueTreeState.getParameter(Parameters::ID_TAIL_LOW_F)->getValue() * 200.0 + 1.0;
    params.lowGainDb = rValueTreeState.getParameter(Parameters::ID_TAIL_LOW_G)->getValue() * 24.0 - 24.0;
    params.highF0 = rValueTreeState.getParameter(Parameters::ID_TAIL_HIGH_F)->getValue() * 10000.0 + 5000.0;
    params.highGainDb = rValueTreeState.getParameter(Parameters::ID_TAIL_HIGH_G)->getValue() * 24.0 - 24.0;
    mFilters.setParameters(params);

    mPath.clear();
    float interval = getWidth() / (float)GRAPH_RESOLUTION;
    for(int i = 0; i < (GRAPH_RESOLUTION + 1); i++)
    {
        float x = i * interval;
        float f = xToF(x);
        float val = mFilters.getMagnitudeAtFrequency(f / 48000.f);
        float y = dbToY(Decibels::gainToDecibels(val));
        if(i == 0)
            mPath.startNewSubPath(x, y);
        else
            mPath.lineTo(x, y);
    }
    repaint();
}