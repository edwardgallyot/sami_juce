#include "sami_WebViewComponent.h"

sami::WebViewComponent::WebViewComponent(bool enableDevTools)
{
    this->webview = std::make_unique<sami::WebViewContainer>(enableDevTools);
    this->webview->AddWebViewToComponent(this);
}

void sami::WebViewComponent::resized()
{
    this->webview->ResizeToComponent(this);
}

void sami::WebViewComponent::SetHTML(const std::string& html)
{
    this->webview->SetHTML(html);
}

void sami::WebViewComponent::SetURL(const std::string& url)
{
    this->webview->SetURL(url);
}
