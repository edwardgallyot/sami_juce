#pragma once

#include "PluginProcessor.h"
#include "GUI/sami_WebViewComponent.h"
#include "GUI/sami_webview_adapter.hpp"


namespace sami {
struct Editor  : public juce::AudioProcessorEditor
{
    explicit Editor (AudioProcessor&);
    ~Editor() override;
private:
    AudioProcessor& p;
    sami::WebViewComponent web;
    sami::adapters::webview_adapter gain_adapter;
    juce::CriticalSection listenerLock;
    void paint (juce::Graphics&) override;
    void resized() override;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Editor)
};

}
