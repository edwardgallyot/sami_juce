#pragma once
#include <JuceHeader.h>
#include <memory>
#include <functional>
#include <concepts>
#include "../GUI/sami_message_parser/target/cxxbridge/sami_message_parser/src/lib.rs.h"
#include "../GUI/sami_WebViewComponent.h"
#include "../PluginProcessor.h"

namespace sami {

namespace adapters {
// A webview adapter represents a 1-1 mapping of a webview component 
// and a processor, it will indiscrimintly recieve callbacks
// from the value tree and the webview when it is registered as a listener to either
// of them. 
//
// This class is also thread safe and realtime safe providing that the functions assigned
// as member callbacks are too.
//
// Updates from registered parameters are polled using a high res timer.
//
// Updates from the webview are triggered from the map within the webview component meaning 
// that this adapter should only recieve updates from the map if needs be. 
struct webview_adapter : public juce::HighResolutionTimer,
                         public juce::AsyncUpdater,
                         public juce::AudioProcessorValueTreeState::Listener,
                         public WebViewComponent::Listener
{
public:
    // A default constructor so you can intialise callbacks later
    webview_adapter(
        WebViewComponent& web,
        sami::AudioProcessor& p,
        const messages::targets::cxx& web_target,
        const juce::String& param_target
    );
    // a callback that is triggered when an update is triggered by the webview
    //
    // N.B. this function could quite easily break the real-time safety of this class.
    // You've been warned...
    std::function<void(const Message&)> webview_callback;

    // a callback that is triggered when an update is polled from the audio thread
    //
    // N.B. this function could quite easily break the real-time safety of this class.
    // You've been warned...
    std::function<void()> processor_callback;

    // Hold some public refs to concrete types
    WebViewComponent& web;
    sami::AudioProcessor& p;
    const messages::targets::cxx& web_target;
    const juce::String& param_target;
private:
    // Interfaces
    // webview listener
    void on_webview_message(Message & message) override;
    // parameter listener
    void parameterChanged(const juce::String &/* parameterID */, float /* newValue */) override;
    // handles an async update from the timer thread
    void handleAsyncUpdate() override;
    // Polls updates from the audio thread
    void hiResTimerCallback() override;

    // Holds whether an update is pending from the audio thread
    // Leave this alone
    std::atomic<bool> should_send_message;
};

// Register the adapters with it's listeners
const auto register_adapters_with_listeners = [](std::same_as<webview_adapter*> auto... adapters) {
    for (auto& adapter : {adapters...}) {
        adapter->p.parameters.addParameterListener(adapter->param_target, adapter);
        adapter->web.listeners.insert({adapter->web_target, adapter});
    }
};

const auto remove_adapters_as_parameter_listeners = [](std::same_as<webview_adapter*> auto... adapters) {
    for (auto& adapter : {adapters...}) {
        adapter->p.parameters.removeParameterListener(adapter->param_target, adapter);
    }
};

const auto start_adapter_timers = [](int ms, std::same_as<webview_adapter*> auto... adapters) {
    for (auto& adapter : {adapters...}) {
        adapter->startTimer(ms);
    }
};

const auto stop_adapter_timers = [](std::same_as<webview_adapter*> auto... adapters) {
    for (auto& adapter : {adapters...}) {
        adapter->stopTimer();
    }
};

}
}

