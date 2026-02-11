#include "Meter.h"

Meter::Meter(MultiChannelMeterProbe& mp) : mVba(this, [&]{ onVblank(); }), rProbe(mp)
{
    mRange = NormalisableRange<float>(-84.0, 6.0, 0.1f, 2.5f);

    String channelNames[2] = { "L", "R" };
    for(int c = 0; c < rProbe.getNumChannels(); c++)
    {
        addChannel(channelNames[c], c);
    }
}

Meter::~Meter()
{

}

void Meter::paint(Graphics& g)
{
    int w = getWidth();
    int h = getHeight();
    int numChannels = mMeterChannels.size();
    float chW = (float)getWidth() / (float)numChannels;
    g.drawImageAt(mBackground, 0, 0);

    //draw channel bars
    ColourGradient barFill(Colour(0xFFAA2700), 0, 0, Colour(0xFF00495C), 0, h, false);
    barFill.addColour(1.f - mRange.convertTo0to1(0.f), Colour(0xFFAA9C00));
    barFill.addColour(1.f - mRange.convertTo0to1(-6.f), Colour(0xFF1D92B7));

    g.setGradientFill(barFill);

    float barW = chW * 0.5;

    for(int c = 0; c < numChannels; c++)
    {
        float barCntX = chW / 2.f + c * chW;
        float barX = barCntX - barW / 2.f;
        float barY = jmap(mRange.convertTo0to1(mMeterChannels[c].currentPeakDb), (float)h, 0.f);
        g.fillRect(barX, barY, barW, h - barY);
    }

    //draw channel labels
    g.setColour(Colour(0x7FFFFFFF));
    for(int c = 0; c < numChannels; c++)
    {
        Rectangle<int> box(c * chW, h - 12.f, chW, 12.f);
        g.setFont(10.f);
        g.drawFittedText(mMeterChannels[c].name, box, Justification::centred, 1);
    }
}

void Meter::resized()
{
    mBackground = Image(Image::PixelFormat::ARGB, getWidth(), getHeight(), true);
    drawBackground();
}

void Meter::drawBackground()
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

    int numChannels = mMeterChannels.size();
    float channelW = w / (float)numChannels;

    for(int i = 0; i < numChannels - 1; i++)
    {
        float x = (float)(i + 1) * channelW;
        g.drawLine(x, 0, x, h);
    }

    float currDb = -6.0;
    float dbInc = -6.0;
    float secLineWidth = 6;
    while(currDb > mRange.start)
    {
        float y = jmap(mRange.convertTo0to1(currDb), h, 0.f);
        float x1 = w / 2.f - secLineWidth / 2.f;
        float x2 = x1 + secLineWidth;
        g.drawLine(x1, y, x2, y);
        currDb += dbInc;
    }
}

void Meter::addChannel(String name, int idx)
{
    jassert(idx >= 0);  // like wtf man

    MeterChannel mc;
    mc.name = name;
    mc.index = idx;
    mc.reset();
    mMeterChannels.push_back(mc);
}

void Meter::onVblank()
{
    updateChannelValues();
    repaint();
}

void Meter::updateChannelValues()
{
    for(int c = 0; c < mMeterChannels.size(); c++)
    {
        float target = Decibels::gainToDecibels(rProbe.getProbe(c).getPeak(true));
        float current = mMeterChannels[c].currentPeakDb;

        current += (target - current) * ((target > current) ? mSmoothUpCoeff : mSmoothDownCoeff);
        mMeterChannels[c].currentPeakDb = jlimit(mRange.start, mRange.end, current);
    }
}