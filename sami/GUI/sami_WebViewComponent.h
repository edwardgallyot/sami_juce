#pragma once

#include <JuceHeader.h>
#include "sami_WebViewContainer.h"

namespace sami
{
    class WebViewComponent : public juce::Component
    {
    public:
        WebViewComponent(bool enableDevTools);
        void resized() override;

        void SetHTML(const std::string& html);
        void SetURL(const std::string& url);

    private:
        std::unique_ptr<WebViewContainer> m_webview;
    };
}
