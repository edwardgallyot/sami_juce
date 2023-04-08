#include "sami_WebViewComponent.h"

sami::WebViewComponent::WebViewComponent(bool enableDevTools)
{
    m_webview = std::make_unique<sami::WebViewContainer>(enableDevTools);
    m_webview->AddWebViewToComponent(this);
}

void sami::WebViewComponent::resized()
{
    m_webview->ResizeToComponent(this);
}

void sami::WebViewComponent::SetHTML(const std::string& html)
{
    m_webview->SetHTML(html);
}

void sami::WebViewComponent::SetURL(const std::string& url)
{
    m_webview->SetURL(url);
}
