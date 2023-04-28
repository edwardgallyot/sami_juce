#pragma once
#include <JuceHeader.h>
#include <functional>
#include "../sami_rs/target/cxxbridge/sami_rs/src/lib.rs.h"

namespace sami {
namespace params {

const juce::String gain = "Gain";
const juce::String attack = "Attack";
const juce::String decay = "Decay";
const juce::String sustain = "Sustain";
const juce::String release = "Release";
const juce::String bypass = "Bypass";

const std::unordered_map<juce::String, std::function<void(Message&)>> param_id_to_target_setter = {
    {gain, messages::targets::set_gain},
    {attack, messages::targets::set_attack},
    {decay, messages::targets::set_decay},
    {sustain, messages::targets::set_sustain},
    {release, messages::targets::set_release},
    {bypass, messages::targets::set_bypass}
};
}
}
