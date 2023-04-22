#pragma once
#include <JuceHeader.h>
#include <functional>
#include "../GUI/sami_message_parser/target/cxxbridge/sami_message_parser/src/lib.rs.h"

namespace sami {
namespace params {

const juce::String gain = "Gain";
const juce::String sustain = "Sustain";
const juce::String bypass = "Bypass";

const std::unordered_map<juce::String, std::function<void(Message&)>> param_id_to_target_setter = {
    {gain, messages::targets::set_gain},
    {sustain, messages::targets::set_sustain},
    {bypass, messages::targets::set_bypass}
};
}
}
