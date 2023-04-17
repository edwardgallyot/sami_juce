#pragma once
#include <JuceHeader.h>
#include <memory>
#include "../GUI/sami_message_parser/target/cxxbridge/sami_message_parser/src/lib.rs.h"

namespace sami {

namespace adapters {

struct webview_adapter : public juce::Timer
{
    std::function<void(const Message&)> on_webview_message;
    std::function<void()> on_plugin_message;
    std::atomic<bool> should_send_message;
private:
    void timerCallback() override;
};

std::unique_ptr<webview_adapter> create_webview_adapter(
    std::function<void(const Message&)> on_webview_message,
    std::function<void()> on_plugin_message
);
    
}
}

