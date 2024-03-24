#include "PluginEditor.h"

NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor(NewProjectAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
      gainSlider(juce::Slider::LinearHorizontal, juce::Slider::NoTextBox),
      pitchShiftSlider(juce::Slider::LinearHorizontal, juce::Slider::NoTextBox)
{
    // Set up gain slider
    gainSlider.setRange(0.0, 1.0, 0.01);
    gainSlider.setValue(0.5); // Set default value
    gainSlider.addListener(this);
    addAndMakeVisible(gainSlider);

    // Set up pitch shift slider
    pitchShiftSlider.setRange(-12.0, 12.0, 0.1);
    pitchShiftSlider.setValue(0.0); // Set default value
    pitchShiftSlider.addListener(this);
    addAndMakeVisible(pitchShiftSlider);

    // Set editor size
    setSize(400, 200);
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor() {}

void NewProjectAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Fill background
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // Draw text
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Gain:", 20, 40, 100, 20, juce::Justification::centred, 1);
    g.drawFittedText("Pitch Shift:", 20, 80, 100, 20, juce::Justification::centred, 1);
}

void NewProjectAudioProcessorEditor::resized()
{
    // Set position and size of sliders
    gainSlider.setBounds(120, 40, 200, 20);
    pitchShiftSlider.setBounds(120, 80, 200, 20);
}

void NewProjectAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &gainSlider)
    {
        // Update gain parameter in audio processor
        audioProcessor.setGain(gainSlider.getValue());
    }
    else if (slider == &pitchShiftSlider)
    {
        // Update pitch shift parameter in audio processor
        audioProcessor.setPitchShift(pitchShiftSlider.getValue());
    }
}
