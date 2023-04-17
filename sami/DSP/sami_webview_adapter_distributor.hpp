#pragma once

#include <JuceHeader.h>
#include "sami_webview_adapter.hpp"

namespace sami {

namespace adapters {
struct webview_distributer {
    std::map<messages::targets::cxx, webview_adapter*> target_to_adapter;
    juce::CriticalSection lock;
};

void add_to_webview_distributor(webview_distributer& dist, messages::targets::cxx target, webview_adapter* adapter);

void clear_webview_distributor(sami::adapters::webview_distributer& dist);
}
}
