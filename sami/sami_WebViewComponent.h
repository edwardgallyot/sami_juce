#pragma once

#include <JuceHeader.h>
#include "sami_WebViewContainer.h"

namespace sami 
{
    class WebViewComponent : public juce::Component
    {
    public:
        WebViewComponent();
        void resized() override;

    private:
        std::unique_ptr<WebViewContainer> m_webview;
    };
}