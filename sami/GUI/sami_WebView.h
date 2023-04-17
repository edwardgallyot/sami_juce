#pragma once

#include "../../choc/gui/choc_WebView.h"
#include <JuceHeader.h>
#include <iostream>

namespace sami
{
struct WebView : public choc::ui::WebView
{
    WebView(bool enableDevTools);
    ~WebView();
    struct Listener
    {
        virtual void on_webview_message(const std::string& msg) = 0;
    };
    void handleWebviewInvocation(const std::string &) override;
    std::vector<Listener*> listener;
};
}
