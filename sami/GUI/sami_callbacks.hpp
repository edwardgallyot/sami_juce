#pragma once
#include "juce_core/system/juce_PlatformDefs.h"
#include "sami_WebViewComponent.h"
#include "../sami_rs/target/cxxbridge/sami_rs/src/lib.rs.h"
#include "../DSP/sami_params.hpp"
#include "sami_webview_adapter.hpp"
#include <JuceHeader.h>
#include <concepts>

namespace sami {
namespace callbacks::webview {
// WEBVIEW SIDE CALLBACKS
// These are triggered when the processor sends an update
using WebViewCallback = std::function<void(const Message&,const sami::AudioProcessor&, const juce::String&)>;

// Callbacks
// =========
// What our adapters expect will be defined
const WebViewCallback maybe_send_float_update = [](
    const Message& message,
    const sami::AudioProcessor& processor,
    const juce::String& param
) {
    float new_val;
    if (messages::updates::get_float(message, new_val)) {
        if (auto* target = processor.parameters.getParameter(param)){
            target->setValueNotifyingHost(target->convertTo0to1(new_val));
        } else {
            DBG(param + "Not Found!");
        }

    }   
};

const WebViewCallback maybe_send_bool_update = [](
    const Message& message,
    const sami::AudioProcessor& processor,
    const juce::String& param
) {
    bool new_val;
    if (messages::updates::get_bool(message, new_val)) {
        if(auto* target = processor.parameters.getParameter(param)) {
            target->setValueNotifyingHost(target->convertTo0to1(new_val));
        } else {
            DBG(param + "Not Found!");
        }
    }   
};

const WebViewCallback maybe_send_gesture_update = [](
    const Message& message,
    const sami::AudioProcessor& processor,
    const juce::String& param
) {
    bool gesture;
    if (messages::updates::get_gesture(message, gesture)) {
        if (auto* target = processor.parameters.getParameter(param)) {
            gesture ?
            target->beginChangeGesture() :
            target->endChangeGesture();
        }
    }
};

// Private functions
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
namespace callbacks::processor {

using ProcessorCallback = std::function<void(const WebViewComponent&,const sami::AudioProcessor&, const juce::String&)>;
// Private function
namespace {
    const auto send_and_destroy_message = [] (const sami::WebViewComponent& web, Message& msg) {
        // Create our stringified payload
        auto payload = std::string(messages::to_js_command(messages::to_json(msg)).c_str());
        web.webview_container->webview->evaluateJavascript(payload);
        messages::destroy(&msg);
    };
    const auto set_target_from_param_id = [] (Message& msg, const juce::String& string) {
        auto found = params::param_id_to_target_setter.find(string);
        if (found != params::param_id_to_target_setter.end())
        {
            found->second(msg);
        }
    };
}

const ProcessorCallback send_float_update = [] (
    const sami::WebViewComponent& web,
    const sami::AudioProcessor& processor,
    const juce::String& paramID
) {
    using namespace sami::messages;
    
    // Create our message and set a target
    auto message = create();
    set_target_from_param_id(*message, paramID);

    // Get our new_value and set the message type
    if (auto* param = processor.parameters.getParameter(paramID)) {
        float new_val = param->convertFrom0to1(param->getValue());
        updates::set_float(*message, new_val);
        send_and_destroy_message(web, *message);
        } else {
            DBG(parmaID + "Not Found!");
        }
};

const ProcessorCallback send_bool_update = [] (
    const sami::WebViewComponent& web,
    const sami::AudioProcessor& processor,
    const juce::String& paramID
) {
    using namespace sami::messages;
    
    // Create our message and set a target
    auto message = create();
    set_target_from_param_id(*message, paramID);

    // Get our new_value and set the message type
    if(auto* param = processor.parameters.getParameter(paramID)) {
        bool new_val = static_cast<bool>(param->convertFrom0to1(param->getValue()));
        updates::set_bool(*message, new_val);
        send_and_destroy_message(web, *message);
    } else {
        DBG(parmaID + "Not Found!");
    }
};

// Private Function
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
