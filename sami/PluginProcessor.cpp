#include "sami_rs/target/cxxbridge/sami_rs/src/lib.rs.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"

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
                juce::ParameterID(sami::params::gain, 1),
                "Gain",
                juce::NormalisableRange<float>{0.0f, 1.0f},
                0.5f
            ),
            std::make_unique<juce::AudioParameterFloat>(
                juce::ParameterID(sami::params::attack, 1),
                "Attack",
                juce::NormalisableRange<float>{0.0f, 1.0f},
                0.5f
            ),
            std::make_unique<juce::AudioParameterFloat>(
                juce::ParameterID(sami::params::decay, 1),
                "Decay",
                juce::NormalisableRange<float>{0.0f, 1.0f},
                0.5f
            ),
            std::make_unique<juce::AudioParameterFloat>(
                juce::ParameterID(sami::params::sustain, 1),
                "Sustain",
                juce::NormalisableRange<float>{0.0f, 1.0f},
                0.5f
            ),
            std::make_unique<juce::AudioParameterFloat>(
                juce::ParameterID(sami::params::release, 1),
                "Release",
                juce::NormalisableRange<float>{0.0f, 1.0f},
                0.5f
            ),
            std::make_unique<juce::AudioParameterBool>(
                juce::ParameterID(sami::params::bypass, 1),
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

//==============================================================================
void sami::AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    this->sampler = std::unique_ptr<sami::sampler::Sampler, std::function<void(sampler::Sampler*)>>(
        sami::sampler::create(sampleRate, static_cast<size_t>(samplesPerBlock)),
        sami::sampler::destroy
    );
}

void sami::AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    // STORE THE I/O Amounts
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // CLEAR THE INPUT CHANNELS
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear (i, 0, buffer.getNumSamples());
    }


    jassert(totalNumInputChannels == 2);

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        // PROCESS SAMI SAMPLER MIDI
        // Process the midid buffer every frame for the sampler
        for (const auto& message : midiMessages) {
            sami::sampler::process_midi(
                *this->sampler,
                message.data,
                static_cast<size_t>(message.numBytes),
                message.samplePosition
            );
        }
        // Also tick along the audio for the sampler
        sami::sampler::process_frame(
            *this->sampler, 
            buffer.getWritePointer(0) + sample,
            buffer.getWritePointer(1) + sample
        );
    }
}

void sami::AudioProcessor::releaseResources()
{
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
