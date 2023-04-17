#pragma once

#include "PluginProcessor.h"
#include "GUI/sami_WebViewComponent.h"
#include "DSP/sami_webview_adapter.hpp"


namespace sami {
struct Editor  : public juce::AudioProcessorEditor,
                 WebView::Listener
{
public:
    explicit Editor (AudioProcessor&);
    ~Editor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void on_webview_message(const std::string &msg) override;

    AudioProcessor& p;
    sami::WebViewComponent web;

    std::unique_ptr<sami::adapters::webview_adapter> gain_adapter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Editor)
};

}
