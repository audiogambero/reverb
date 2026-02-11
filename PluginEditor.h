#pragma once

#include "UI/StatusBar.h"
// #include "PluginProcessor.h"
#include "UI/Widgets/Knob.h"
#include "UI/Widgets/Meter.h"
#include "UI/Widgets/EQGraph.h"
#include "Core/Parameters.h"
#include "UI/NeverbLookAndFeel.h"

//==============================================================================
class AudioPluginAudioProcessorEditor final : public AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    AudioPluginAudioProcessor& processorRef;
    Image mBackground;

    std::unique_ptr<Meter> pMeter;

    std::unique_ptr<ToggleButton> pButtonDiffusion;

    std::unique_ptr<Knob> pKnobPredelay;

    std::unique_ptr<Knob> pKnobDecay;
    std::unique_ptr<Knob> pKnobDamp;
    std::unique_ptr<Knob> pKnobModulation;
    std::unique_ptr<Knob> pKnobScale;

    std::unique_ptr<Knob> pKnobLowF;
    std::unique_ptr<Knob> pKnobLowG;
    std::unique_ptr<Knob> pKnobHighF;
    std::unique_ptr<Knob> pKnobHighG;

    std::unique_ptr<Knob> pKnobMix;
    
    std::unique_ptr<EQGraph> pEQGraph;

    std::unique_ptr<StatusBar> pStatusBar;

    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> pAttachmentDiffusion;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> pAttachmentPredelay;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> pAttachmentDecay;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> pAttachmentDamp;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> pAttachmentModulation;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> pAttachmentScale;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> pAttachmentLowF;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> pAttachmentLowG;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> pAttachmentHighF;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> pAttachmentHighG;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> pAttachmentMix;


    NeverbLookAndFeel laf;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
