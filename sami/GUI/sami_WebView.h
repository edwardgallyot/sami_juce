#pragma once

#include "../../choc/gui/choc_WebView.h"
#include <JuceHeader.h>
#include <iostream>

namespace sami
{
class WebView : public choc::ui::WebView
{
public:
    WebView(bool enableDevTools);
    ~WebView();

    struct Listener
    {
        Listener(const std::string& id);
        virtual void OnWebViewInvocation(const std::string& msg) = 0;
        std::string& id;
    };

    void handleWebviewInvocation(const std::string &) override;
private:
    std::vector<Listener*> listener;
    
};
}
