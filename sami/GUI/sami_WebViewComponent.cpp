#include "sami_WebViewComponent.h"

sami::WebViewComponent::WebViewComponent(bool enableDevTools)
{
    this->webview_container = std::make_unique<sami::WebViewContainer>(enableDevTools);
    this->webview_container->AddWebViewToComponent(this);
}

void sami::WebViewComponent::resized()
{
    this->webview_container->ResizeToComponent(this);
}
