#include "StatusBar.h"

StatusBar::StatusBar(AudioPluginAudioProcessor& p) : rProc(p)
{
    auto& apvts = rProc.getValueTreeState();
    apvts.addParameterListener(Parameters::ID_PREDELAY, this);
    apvts.addParameterListener(Parameters::ID_INPUT_DIFFUSION, this);
    apvts.addParameterListener(Parameters::ID_DECAY, this);
    apvts.addParameterListener(Parameters::ID_DAMP, this);
    apvts.addParameterListener(Parameters::ID_MODULATION, this);
    apvts.addParameterListener(Parameters::ID_SCALE, this);
    apvts.addParameterListener(Parameters::ID_TAIL_LOW_F, this);
    apvts.addParameterListener(Parameters::ID_TAIL_LOW_G, this);
    apvts.addParameterListener(Parameters::ID_TAIL_HIGH_F, this);
    apvts.addParameterListener(Parameters::ID_TAIL_HIGH_G, this);
    apvts.addParameterListener(Parameters::ID_MIX, this);

    mDisplayText = "hello :)";
    startTimer(1000);
}

StatusBar::~StatusBar()
{

}

void StatusBar::paint(Graphics& g)
{
    g.fillAll(Colours::black);
    g.setColour(Colours::white);
    g.drawText(mDisplayText, 8, 0 , getWidth() , getHeight(), Justification::centredLeft, true);
}

void StatusBar::resized()
{

}

void StatusBar::parameterChanged(const String& parameterId, float newValue)
{
    auto& apvts = rProc.getValueTreeState();
    auto param = apvts.getParameter(parameterId);
    stopTimer();

    mDisplayText = String(param->getName(64)) + String(": ") + param->getCurrentValueAsText();
    startTimer(3e3);
    repaint();
}

void StatusBar::timerCallback()
{
    mDisplayText = juce::String::formatted("%s by %s (ver. %s)", 
                                       JucePlugin_Name, 
                                       JucePlugin_Manufacturer, 
                                       JucePlugin_VersionString);
    stopTimer();
    repaint();
}