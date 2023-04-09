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
    if (juce::MessageManager::getInstanceWithoutCreating()->isThisTheMessageThread()) {
    } else {
        // We don't know how we will handle this if it's not on another thread at the moment.
        // At least we can assert here and figure out what's going on later...
        jassertfalse;
    }
}
