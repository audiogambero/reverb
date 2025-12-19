#pragma once

#include <JuceHeader.h>
#include "../../Shared/MultiChannelMeterProbe.h"
#include <vector>

struct MeterChannel
{
    int index;     // Audio channel index
    String name;

    float currentPeakDb = -80.f;

    void reset() { currentPeakDb = -120.f; }
    
    bool operator== (const MeterChannel& rhs) const {
        return name.trim() == rhs.name.trim() || index == rhs.index;      //"L" and "L " are the same channel
    }
};

class Meter : public Component
{
public:
    Meter(MultiChannelMeterProbe& mp);
    ~Meter() override;

    void paint(Graphics& g) override;
    void resized() override;

    void addChannel(String name, int idx);
    void setSmoothing(float up, float down);

    void onVblank();
private:
    VBlankAttachment mVba;
    Image mBackground;

    NormalisableRange<float> mRange;
    MultiChannelMeterProbe& rProbe;
    std::vector<MeterChannel> mMeterChannels;

    float mSmoothUpCoeff = 0.5f;
    float mSmoothDownCoeff = 0.1f;

    void drawBackground();
    void updateChannelValues();
};