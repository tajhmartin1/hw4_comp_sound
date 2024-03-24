/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessor::NewProjectAudioProcessor()
: AudioProcessor(BusesProperties()
    .withInput("Mono In 1", juce::AudioChannelSet::mono(), true)
    .withOutput("Mono Out 1", juce::AudioChannelSet::mono(), true))
        //.withOutput("Mono Out 2", juce::AudioChannelSet::mono(), true)
        //.withOutput("Mono Out 3", juce::AudioChannelSet::mono(), true))

        //#ifndef JucePlugin_PreferredChannelConfigurations
//     : AudioProcessor (BusesProperties()
//                     #if ! JucePlugin_IsMidiEffect
//                      #if ! JucePlugin_IsSynth
//                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
//                      #endif
//                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
//                     #endif
//                       )
//#endif
{
}

NewProjectAudioProcessor::~NewProjectAudioProcessor()
{
}

//==============================================================================
const juce::String NewProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NewProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NewProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NewProjectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NewProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NewProjectAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NewProjectAudioProcessor::getProgramName (int index)
{
    return {};
}

void NewProjectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

float saw(float t) {
    double fullPeriodTime = 1.0 / 220;
    double localTime = fmod(t, fullPeriodTime);

    return 0.3   * ((localTime / fullPeriodTime) * 2 - 1.0);
};

float sinFxn(float x) { return std::sin(x); }

void setP(juce::dsp::ProcessorChain<juce::dsp::Oscillator<float>, juce::dsp::Gain<float>> &pChain,
    juce::dsp::ProcessSpec spec,
    float freq) {
    auto& osc = pChain.template get<0>();
    osc.initialise(saw, 128);
    osc.setFrequency(freq, false);
    auto& gain = pChain.template get<1>();
    gain.setGainLinear(0.01f);
    pChain.prepare(spec);
}
//==============================================================================
void NewProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();

    for (int i = 0; i < 24; i++) {
        setP(processorChains[i], spec, 220.0f);// +((float)i * 0.1f));
    }

}
void NewProjectAudioProcessor::generateBeep(juce::AudioBuffer<float>& buffer)
{
    const double sampleRate = getSampleRate();
    const double frequency = 1000.0; // Frequency of the beep in Hz
    const double amplitude = 0.5; // Amplitude of the beep (adjust as needed)
    const double twoPi = 2.0 * juce::MathConstants<double>::pi;

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            // Calculate the value of the sine wave at the current sample
            double value = amplitude * std::sin(twoPi * frequency * sample / sampleRate);

            // Write the value to the buffer
            channelData[sample] = static_cast<float>(value);
        }
    }
}

//void NewProjectAudioProcessor::setGain(float gainValue)
//{
//    // Clamp gain value to the range [0.0, 1.0]
//    float clampedGain = juce::jlimit(0.0f, 1.0f, gainValue);
//
//    // Update the gain parameter
//    gain = clampedGain;
//}
//
//void NewProjectAudioProcessor::setPitchShift(float pitchShiftValue)
//{
//    // Clamp pitch shift value to the range [-12.0, 12.0]
//    float clampedPitchShift = juce::jlimit(-12.0f, 12.0f, pitchShiftValue);
//
//    // Update the pitch shift parameter
//    pitchShift = clampedPitchShift;
//}



void NewProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NewProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return true;
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif
void NewProjectAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // Generate the beep internally
    generateBeep(buffer);

    // Apply gain to the audio samples
    buffer.applyGain(gain);

    // Ensure the pitch shift value is within bounds [-12.0, 12.0]
    float clampedPitchShift = juce::jlimit(-12.0f, 12.0f, pitchShift);

    // Apply pitch shift to the audio samples
    if (clampedPitchShift != 0.0f)
    {
        // Iterate over each channel in the audio buffer
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            // Get a pointer to the audio samples for the current channel
            float* channelData = buffer.getWritePointer(channel);

            // Calculate the number of samples in the current channel
            int numSamples = buffer.getNumSamples();

            // Apply pitch shift to the samples in the current channel
            // Adjust pitch shift algorithm based on your requirements
            for (int sample = 0; sample < numSamples; ++sample)
            {
                // Apply pitch shift algorithm here
                // For simplicity, let's just change the playback speed
                channelData[sample] *= std::pow(2.0f, clampedPitchShift / 12.0f);
            }
        }
    }
}


//==============================================================================
bool NewProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NewProjectAudioProcessor::createEditor()
{
    return new NewProjectAudioProcessorEditor (*this);
}

//==============================================================================
void NewProjectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void NewProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}
