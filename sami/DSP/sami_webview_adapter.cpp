#include "sami_webview_adapter.hpp"
#include <functional>
#include <memory>

void sami::adapters::webview_adapter::timerCallback() {
    if (should_send_message.exchange(false)) {
        this->on_plugin_message();
    }
}

// Creates a webview adapter ready for use
std::unique_ptr<sami::adapters::webview_adapter> sami::adapters::create_webview_adapter(
    std::function<void(const Message&)> on_webview_message,
    std::function<void()> on_plugin_message
) {
    auto adapter = new webview_adapter;
    adapter->on_webview_message = on_webview_message;
    adapter->on_plugin_message = on_plugin_message;
    adapter->should_send_message = false;
    adapter->startTimerHz(60);
    return std::unique_ptr<sami::adapters::webview_adapter>(adapter);
}
