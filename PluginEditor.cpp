#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    auto& apvts = processorRef.getValueTreeState();

    mBackground = ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);

    pButtonDiffusion = std::make_unique<ToggleButton>();

    pKnobPredelay = std::make_unique<Knob>();
    pKnobDecay = std::make_unique<Knob>();
    pKnobDamp = std::make_unique<Knob>();
    pKnobModulation = std::make_unique<Knob>();
    pKnobScale = std::make_unique<Knob>();
    pKnobLowF = std::make_unique<Knob>();
    pKnobLowG = std::make_unique<Knob>();
    pKnobHighF = std::make_unique<Knob>();
    pKnobHighG = std::make_unique<Knob>();
    pKnobMix = std::make_unique<Knob>();

    pMeter = std::make_unique<Meter>(processorRef.getProbe());

    pEQGraph = std::make_unique<EQGraph>();

    addAndMakeVisible(pButtonDiffusion.get());

    addAndMakeVisible(pKnobPredelay.get());
    addAndMakeVisible(pKnobDecay.get());
    addAndMakeVisible(pKnobDamp.get());
    addAndMakeVisible(pKnobModulation.get());
    addAndMakeVisible(pKnobScale.get());
    addAndMakeVisible(pKnobLowF.get());
    addAndMakeVisible(pKnobLowG.get());
    addAndMakeVisible(pKnobHighF.get());
    addAndMakeVisible(pKnobHighG.get());
    addAndMakeVisible(pKnobMix.get());

    addAndMakeVisible(pMeter.get());

    addAndMakeVisible(pEQGraph.get());

    pAttachmentDiffusion = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(apvts, Parameters::ID_INPUT_DIFFUSION, *pButtonDiffusion.get());
    pAttachmentPredelay = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(apvts, Parameters::ID_PREDELAY, *pKnobPredelay.get());
    pAttachmentDecay = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(apvts, Parameters::ID_DECAY, *pKnobDecay.get());
    pAttachmentDamp = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(apvts, Parameters::ID_DAMP, *pKnobDamp.get());
    pAttachmentModulation = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(apvts, Parameters::ID_MODULATION, *pKnobModulation.get());
    pAttachmentScale = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(apvts, Parameters::ID_SCALE, *pKnobScale.get());
    pAttachmentLowF = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(apvts, Parameters::ID_TAIL_LOW_F, *pKnobLowF.get());
    pAttachmentLowG = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(apvts, Parameters::ID_TAIL_LOW_G, *pKnobLowG.get());
    pAttachmentHighF = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(apvts, Parameters::ID_TAIL_HIGH_F, *pKnobHighF.get());
    pAttachmentHighG = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(apvts, Parameters::ID_TAIL_HIGH_G, *pKnobHighG.get());
    pAttachmentMix = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(apvts, Parameters::ID_MIX, *pKnobMix.get());

    setSize (1000, 520);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (Graphics& g)
{
    g.drawImage(mBackground, getLocalBounds().toFloat());
}

void AudioPluginAudioProcessorEditor::resized()
{
    pButtonDiffusion->setBounds(97, 64, 48, 24);

    pKnobPredelay->setBounds(81, 120, 80, 80);

    pKnobDecay->setBounds(240, 40, 160, 160);
    pKnobDamp->setBounds(440, 80, 80, 80);
    pKnobModulation->setBounds(560, 80, 80, 80);
    pKnobScale->setBounds(680, 80, 80, 80);

    pKnobMix->setBounds(840, 80, 80, 80);

    pKnobLowF->setBounds(80, 360, 80, 80);
    pKnobLowG->setBounds(200, 360, 80, 80);
    pKnobHighF->setBounds(560, 360, 80, 80);
    pKnobHighG->setBounds(680, 360, 80, 80);

    pMeter->setBounds(840, 320, 80, 160);

    pEQGraph->setBounds(320, 320, 200, 160);
}
