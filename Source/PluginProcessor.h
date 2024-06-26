/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class NewProjectAudioProcessor  : public juce::AudioProcessor
{
public:
    void generateBeep(juce::AudioBuffer<float>& buffer);

    void setGain(float gainValue)
       {
           // Clamp gain value to the range [0.0, 1.0]
           float clampedGain = juce::jlimit(0.0f, 1.0f, gainValue);

           // Update the gain parameter
           gain = clampedGain;
       }

       void setPitchShift(float pitchShiftValue)
       {
           // Clamp pitch shift value to the range [-12.0, 12.0]
           float clampedPitchShift = juce::jlimit(-12.0f, 12.0f, pitchShiftValue);

           // Update the pitch shift parameter
           pitchShift = clampedPitchShift;
       }

    //==============================================================================
    NewProjectAudioProcessor();
    ~NewProjectAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void sliderValueChanged(juce::Slider* slider) ;

private:
    float gain = 1.0f;
    float pitchShift = 0.0f;
    //==============================================================================
    
    enum
    {
        oscIndex,
        gainIndex
    };

    juce::dsp::ProcessorChain<juce::dsp::Oscillator<float>, juce::dsp::Gain<float>> processorChains[24];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessor)
};
