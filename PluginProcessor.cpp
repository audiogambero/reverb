#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
        mValueTreeState(*this, &mUndoManager, juce::Identifier("NeverbParams"), Parameters::createLayout())
{
    
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
}

//==============================================================================
const String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram (int index)
{
    ignoreUnused (index);
}

const String AudioPluginAudioProcessor::getProgramName (int index)
{
    ignoreUnused (index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName (int index, const String& newName)
{
    ignoreUnused (index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    ignoreUnused(samplesPerBlock);
    mReverbTank.reset(sampleRate);
    mReverbTank.setParameters(mReverbTank.getParameters());
    inputFrame.resize(getNumInputChannels());
    outputFrame.resize(getNumOutputChannels());
    mProbe.setNumChannels(getNumOutputChannels());
    prepared = true;
}

void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;
    
    auto inputSet = layouts.getMainInputChannelSet();
    if (inputSet != AudioChannelSet::mono() && inputSet != AudioChannelSet::stereo())
        return false;

    return true;
}

void AudioPluginAudioProcessor::processBlock (AudioBuffer<float>& buffer,
                                              MidiBuffer& midiMessages)
{
    ignoreUnused (midiMessages);

    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto params = mReverbTank.getParameters();
    params.feedback = mValueTreeState.getParameter(Parameters::ID_DECAY)->getValue();
    params.modAmount = mValueTreeState.getParameter(Parameters::ID_MODULATION)->getValue();
    params.lpfG = mValueTreeState.getParameter(Parameters::ID_DAMP)->getValue();
    params.preDelayMs = mValueTreeState.getParameter(Parameters::ID_PREDELAY)->getValue() * 250.0;
    params.mix = mValueTreeState.getParameter(Parameters::ID_MIX)->getValue();
    params.maxApfMs = 100.0;
    params.maxDelayMs = 80.1;
    float scale = mValueTreeState.getParameter(Parameters::ID_SCALE)->getValue() * 0.6 + 0.4;
    params.weightApf = 1.0 * scale;
    params.weightDelay = 1.0 * scale;
    params.diffuseInput = mValueTreeState.getParameter(Parameters::ID_INPUT_DIFFUSION)->getValue() != 0.f;
    params.tailShelfHighF = mValueTreeState.getParameter(Parameters::ID_TAIL_HIGH_F)->getValue() * 10000.0 + 5000.0;
    params.tailShelfHighGain = mValueTreeState.getParameter(Parameters::ID_TAIL_HIGH_G)->getValue() * 24.0 - 24.0;
    params.tailShelfLowF = mValueTreeState.getParameter(Parameters::ID_TAIL_LOW_F)->getValue() * 200.0 + 1.0;
    params.tailShelfLowGain = mValueTreeState.getParameter(Parameters::ID_TAIL_LOW_G)->getValue() * 24.0 - 24.0;
    mReverbTank.setParameters(params);

    if(prepared)
    {
        for(int s = 0; s < buffer.getNumSamples(); s++)
        {
            for (int channel = 0; channel < totalNumInputChannels; ++channel)
            {
                inputFrame[channel] = buffer.getReadPointer(channel)[s];
            }
    
            mReverbTank.processAudioFrame(
                inputFrame.data(),    // input frame
                outputFrame.data(),   // output frame
                totalNumInputChannels,
                totalNumOutputChannels
            );

            mProbe.pushFrame(outputFrame.data(), totalNumOutputChannels);
    
            // Write processed output back to buffer
            for (int channel = 0; channel < totalNumOutputChannels; ++channel)
            {
                buffer.getWritePointer(channel)[s] = outputFrame[channel];
            }
        }
    }
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor (*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state = mValueTreeState.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (mValueTreeState.state.getType()))
            mValueTreeState.replaceState (juce::ValueTree::fromXml (*xmlState));
}

AudioProcessorValueTreeState& AudioPluginAudioProcessor::getValueTreeState()
{
    return mValueTreeState;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}
