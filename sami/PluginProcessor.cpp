#include "PluginProcessor.h"
#include "GUI/sami_message_parser/target/cxxbridge/sami_message_parser/src/lib.rs.h"
#include "PluginEditor.h"
#include "DSP/sami_params.hpp"
#include <iostream>

//==============================================================================
sami::AudioProcessor::AudioProcessor() 
     : juce::AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    parameters(
        *this,
        nullptr,
        juce::Identifier("APVTSSAMI"),
        {
            std::make_unique<juce::AudioParameterFloat>(
                sami::params::gain,
                "Gain",
                juce::NormalisableRange<float>{0.0f, 1.0f},
                0.5f
            ),
            std::make_unique<juce::AudioParameterFloat>(
                sami::params::sustain,
                "Sustain",
                juce::NormalisableRange<float>{0.0f, 1.0f},
                0.5f
            ),
            std::make_unique<juce::AudioParameterBool>(
                "bypass",
                "Bypass",
                false
            ),
        }    
    )
{
}

sami::AudioProcessor::~AudioProcessor()
{
}

void sami::AudioProcessor::parameterChanged(const juce::String& parameterID, float /* newValue */) {
    // Search our target map for our ID
    auto result = sami::params::param_id_to_target.find(parameterID);
    // If we've got a target
    if (result != sami::params::param_id_to_target.end()) {
        // Search the distributor
        auto dist = webview_distributer.target_to_adapter.find(result->second);
        // If the distributor is ready
        if (dist != webview_distributer.target_to_adapter.end()) {
            // Set out atomic flag in the adapter
            dist->second->should_send_message.store(true);
        }
    }

}

//==============================================================================
// JUCE BOILER PLATE!
const juce::String sami::AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool sami::AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool sami::AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool sami::AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double sami::AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int sami::AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int sami::AudioProcessor::getCurrentProgram()
{
    return 0;
}

void sami::AudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String sami::AudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void sami::AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void sami::AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void sami::AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool sami::AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
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

void sami::AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        juce::ignoreUnused (channelData);
        // ..do something to the data...
    }
}

//==============================================================================
bool sami::AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* sami::AudioProcessor::createEditor()
{
    return new sami::Editor (*this);
}

//==============================================================================
void sami::AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void sami::AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new sami::AudioProcessor();
}
