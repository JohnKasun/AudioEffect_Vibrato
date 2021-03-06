#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor(&p), processorRef(p), mValueTreeState(vts)
{
    juce::ignoreUnused(processorRef);

    addAndMakeVisible(mFreqSlider);
    mFreqAttachment.reset(new SliderAttachment(mValueTreeState, "freq", mFreqSlider));
    mFreqSlider.setName("Speed");
    mFreqSlider.setLookAndFeel(&mMyLookAndFeel);
    mFreqSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    mFreqSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, paramControlWidth, paramLabelHeight);

    addAndMakeVisible(mDepthSlider);
    mDepthAttachment.reset(new SliderAttachment(mValueTreeState, "depth", mDepthSlider));
    mDepthSlider.setName("Depth");
    mDepthSlider.setLookAndFeel(&mMyLookAndFeel);
    mDepthSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    mDepthSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, paramControlWidth, paramLabelHeight);

    setSize(paramControlWidth * 2, paramControlHeight + paramLabelHeight);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void AudioPluginAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    auto freqArea = area.removeFromLeft(paramControlWidth);

    mFreqSlider.setBounds(freqArea);
    mDepthSlider.setBounds(area);
}