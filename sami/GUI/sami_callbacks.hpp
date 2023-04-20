#pragma once
#include "sami_message_parser/target/cxxbridge/sami_message_parser/src/lib.rs.h"
#include "../DSP/sami_params.hpp"
#include "sami_webview_adapter.hpp"
#include <JuceHeader.h>
#include <concepts>

namespace sami {
namespace callbacks {

// WEBVIEW SIDE CALLBACKS
// These are triggered when the processor sends an update
namespace webview {

// Callbacks
// =========
// What our adapters expect will be defined
const auto maybe_send_float_update = [](
    const Message& message,
    const sami::AudioProcessor& processor,
    const juce::String& param
) {
    float new_val = 0.0f;
    if (messages::updates::get_float(message, new_val)) {
        auto* target = processor.parameters.getParameter(param);
        target->setValueNotifyingHost(target->convertTo0to1(new_val));
    }   
};
namespace {
const auto process_messages = [](
    std::tuple<const Message&, const sami::AudioProcessor&, const juce::String&> data,
    const std::invocable<const Message&,const sami::AudioProcessor&, const juce::String&> auto& ...callbacks
    ) {
    for (const auto& callback : {callbacks...}) {
        callback(
            std::get<const Message&>(data),
            std::get<const sami::AudioProcessor&>(data),
            std::get<const juce::String&>(data) 
        );
    }
};

}

const auto register_callbacks_with_adapter = [] (sami::adapters::webview_adapter& adapter,
    const std::invocable<const Message&,const sami::AudioProcessor&, const juce::String&> auto& ...callbacks
    ){
    adapter.webview_callback = [&] (const Message& message) {
        process_messages(
            {message, adapter.p, adapter.param_target},
            callbacks...
        );
    };
};
}

// PROCESSOR SIDE CALLBACKS
// These are triggered when the processor sends an update
namespace processor {

const auto send_float_update = [] (
    const sami::WebViewComponent& web,
    const sami::AudioProcessor& processor,
    const juce::String& paramID
) {
    using namespace sami::messages;
    
    // Create our message and set a target
    auto message = create();

    // Find our target setter from the rust lib and set the target if it exists
    auto found = params::param_id_to_target_setter.find(paramID);
    if (found != params::param_id_to_target_setter.end())
    {
        found->second(*message);
    }
    
    // Get our new_value and set the message type
    auto* param = processor.parameters.getParameter(paramID);
    float new_val = param->convertFrom0to1(param->getValue());
    updates::set_float(*message, new_val);
    
    // Create our stringified payload
    auto payload = std::string(to_js_command(to_json(*message)).c_str());
    web.webview_container->webview->evaluateJavascript(payload);
    destroy(message);
    
};

namespace {
const auto process_messages = [](
    std::tuple<const sami::WebViewComponent&, const sami::AudioProcessor&, const juce::String&> data,
    const std::invocable<const sami::WebViewComponent&, const sami::AudioProcessor&, const juce::String&> auto& ...callbacks
    ) {
    for (const auto& callback : {callbacks...}) {
        callback(
            std::get<const WebViewComponent&>(data),
            std::get<const sami::AudioProcessor&>(data),
            std::get<const juce::String&>(data) 
        );
    }
};
} 

const auto register_callbacks_with_adapter = [] (sami::adapters::webview_adapter& adapter,
    const std::invocable<const sami::WebViewComponent&, const sami::AudioProcessor&, const juce::String&> auto& ...callbacks
    ){
    adapter.processor_callback = [&] {
        process_messages(
            {adapter.web, adapter.p, adapter.param_target},
            callbacks...
        );
    };
};
}
}
}
