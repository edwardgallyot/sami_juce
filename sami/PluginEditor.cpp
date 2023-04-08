#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), 
    processorRef (p),
    m_web(true)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setResizable(true, true);
    setSize(100, 100);
    addAndMakeVisible(m_web);
    m_web.SetURL("http://localhost:5173");
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

void AudioPluginAudioProcessorEditor::paint (juce::Graphics& )
{
    /* empty implementation */
}

void AudioPluginAudioProcessorEditor::resized()
{
    m_web.setBounds(getLocalBounds());
}
