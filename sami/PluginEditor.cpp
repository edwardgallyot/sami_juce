#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setResizable(true, true);
    setSize(100, 100);
    addAndMakeVisible(m_web);

//    const char* html =
//        "<!DOCTYPE html>"
//        "<html>"
//            "<head>"
//                "<title>Example</title>"
//            "</head>"
//            "<body>"
//                "<p>This is an example of a simple HTML page with one paragraph.</p>"
//            "</body>"
//        "</html>";
//    m_web.SetHTML(html);
    m_web.SetURL("http://localhost:5173");
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

void AudioPluginAudioProcessorEditor::paint (juce::Graphics& )
{
}

void AudioPluginAudioProcessorEditor::resized()
{
    m_web.setBounds(getLocalBounds());
}
