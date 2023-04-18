#pragma once

#include "GUI/sami_message_parser/target/cxxbridge/sami_message_parser/src/lib.rs.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include <unordered_map>

namespace sami {
struct AudioProcessor  : public juce::AudioProcessor
{
    AudioProcessor();
    ~AudioProcessor() override;

    juce::AudioProcessorValueTreeState parameters;

    // Underneath is JUCE boiler plate code that is seperated away from the functional style
    // of sami. We want to keep this seperate to avoid too many OOP additions.

    // JUCE BOILER PLATE
    // ==================================================
private:
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioProcessor)
};

}
