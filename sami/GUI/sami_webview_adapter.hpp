#pragma once
#include <JuceHeader.h>
#include <memory>
#include <functional>
#include "../GUI/sami_message_parser/target/cxxbridge/sami_message_parser/src/lib.rs.h"
#include "../GUI/sami_WebViewComponent.h"

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
    // A default constructor so you can intialise callbacks later
    webview_adapter();
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

std::unique_ptr<webview_adapter> create_webview_adapter(
    std::function<void(const Message&)> on_webview_message,
    std::function<void()> on_plugin_message
);
    
}
}

