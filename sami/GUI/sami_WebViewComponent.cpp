#include "sami_WebViewComponent.h"
#include "sami_message_parser/target/cxxbridge/sami_message_parser/src/lib.rs.h"

sami::WebViewComponent::WebViewComponent(bool enableDevTools)
{
    this->webview_container = std::make_unique<sami::WebViewContainer>(enableDevTools);
    this->webview_container->AddWebViewToComponent(this);
    this->webview_container->webview->listeners.push_back(this);
}

void sami::WebViewComponent::resized()
{
    this->webview_container->ResizeToComponent(this);
}

void sami::WebViewComponent::on_webview_message(const std::string &msg) {
    using namespace sami::messages;
    // Deserialise the JSON
    auto message = create(msg);

    // If we're receiving an init message from the webview we need an intial update
    if (message_types::get(*message) == message_types::cxx::init){
        for (const auto& [_, listener] : this->listeners){
            listener->on_webview_init();
        }
    }

    // Find the target
    auto target = targets::get(*message);

    // Send him on his way
    if (this->listeners.find(target) != this->listeners.end()) {
        this->listeners.at(target)->on_webview_message(*message);
    }
}
