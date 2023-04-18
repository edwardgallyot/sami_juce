#pragma once

#include <JuceHeader.h>
#include "sami_WebViewContainer.h"
#include "sami_message_parser/target/cxxbridge/sami_message_parser/src/lib.rs.h"

namespace sami
{
struct WebViewComponent : public juce::Component,
                          WebView::Listener {
    struct Listener {
        virtual void on_webview_message(Message&) = 0;
    };
    WebViewComponent(bool enableDevTools);
    std::unique_ptr<WebViewContainer> webview_container;
    std::map<sami::messages::targets::cxx, Listener*> listeners;
private:
    void on_webview_message(const std::string &msg) override;
    void resized() override;
};
}
