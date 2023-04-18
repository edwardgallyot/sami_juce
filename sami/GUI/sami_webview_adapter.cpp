#include "sami_webview_adapter.hpp"

sami::adapters::webview_adapter::webview_adapter() {
    this->should_send_message = false;
}

void sami::adapters::webview_adapter::hiResTimerCallback() {
    if (should_send_message.exchange(false)) {
        this->triggerAsyncUpdate();
    }
}
void sami::adapters::webview_adapter::parameterChanged(const juce::String &/* parameterID */, float /* newValue */) {
    this->should_send_message.store(true);
}

void sami::adapters::webview_adapter::handleAsyncUpdate() {
    this->processor_callback();
}

void sami::adapters::webview_adapter::on_webview_message(Message& message) {
    this->webview_callback(message);
}
