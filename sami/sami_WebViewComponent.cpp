#include "sami_WebViewComponent.h"

sami::WebViewComponent::WebViewComponent()
{
    m_webview = std::make_unique<sami::WebViewContainer>();
    addAndMakeVisible(m_webview->GetComponent());
}

void sami::WebViewComponent::resized()
{
    m_webview->GetComponent()->setBounds(getLocalBounds());
}
