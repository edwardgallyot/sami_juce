#pragma once

#include <JuceHeader.h>
#include "sami_WebViewContainer.h"

namespace sami
{
struct WebViewComponent : public juce::Component
{
    WebViewComponent(bool enableDevTools);
    void resized() override;
    std::unique_ptr<WebViewContainer> webview_container;
};
}
