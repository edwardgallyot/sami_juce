#include <JuceHeader.h>
#include "../GUI/sami_message_parser/target/cxxbridge/sami_message_parser/src/lib.rs.h"

namespace sami {
namespace params {

const juce::String gain = "Gain";
const juce::String sustain = "Sustain";

const std::unordered_map<juce::String, messages::targets::cxx> param_id_to_target = {
    {gain, messages::targets::cxx::gain},
    {sustain, messages::targets::cxx::sustain}
};

}
}
