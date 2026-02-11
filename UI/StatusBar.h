#pragma once

#include "../PluginProcessor.h"

class StatusBar : public Component, public AudioProcessorValueTreeState::Listener, public Timer
{
public:
    StatusBar(AudioPluginAudioProcessor& p);
    ~StatusBar() override;

    void paint(Graphics& g) override;
    void resized() override;

    void parameterChanged(const String& parameterId, float newValue) override;

    void timerCallback() override;
private:
    AudioPluginAudioProcessor& rProc;
    String mDisplayText;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StatusBar)
};