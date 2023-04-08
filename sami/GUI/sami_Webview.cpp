#include "sami_WebView.h"

sami::WebView::WebView(bool enableDevTools)
    : choc::ui::WebView({
        enableDevTools, 
        [](const std::string&){ return std::nullopt;}
    })
{
}

sami::WebView::~WebView()
{
}
void sami::WebView::handleWebviewInvocation(const std::string & msg)
{
    DBG(msg);
}
