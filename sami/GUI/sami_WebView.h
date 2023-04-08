#pragma once

#include "../../choc/gui/choc_WebView.h"
#include <JuceHeader.h>

namespace sami
{
class WebView : public choc::ui::WebView
{
public:
    WebView(bool enableDevTools);
    ~WebView();

    void handleWebviewInvocation(const std::string &) override;

private:
    
};
}
