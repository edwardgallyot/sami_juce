#include "sami_WebView.h"
#include "../proto/messages.pb.h"
#include <memory>

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
        auto message = std::make_unique<sami::Test>();
        message->ParseFromString(msg);
        std::cout << message->message() << std::endl; 
    } else {
        // We don't know how we will handle this if it's not on another thread at the moment.
        // At least we can assert here and figure out what's going on later...
        jassertfalse;
    }
}
