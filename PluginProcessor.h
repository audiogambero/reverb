#pragma once

#include <JuceHeader.h>
#include "DSP/ReverbTank.h"
#include "Shared/MultiChannelMeterProbe.h"
#include "Core/Parameters.h"

//==============================================================================
class AudioPluginAudioProcessor final : public AudioProcessor
{
public:
    //==============================================================================
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    AudioProcessorValueTreeState& getValueTreeState();
    UndoManager& getUndoManager();

    MultiChannelMeterProbe& getProbe() 
    {
        return mProbe;
    }

private:
    AudioProcessorValueTreeState mValueTreeState;
    UndoManager mUndoManager;

    ReverbTank mReverbTank;
    MultiChannelMeterProbe mProbe;
    bool prepared;

    std::vector<float> inputFrame, outputFrame;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)
};
