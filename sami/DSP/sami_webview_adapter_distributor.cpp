#include "sami_webview_adapter_distributor.hpp"
#include <memory>

void sami::adapters::add_to_webview_distributor(sami::adapters::webview_distributer& dist, messages::targets::cxx target, sami::adapters::webview_adapter* adapter) {
    auto scope = juce::ScopedLock(dist.lock);
    dist.target_to_adapter.insert({target, adapter});
}

void sami::adapters::clear_webview_distributor(sami::adapters::webview_distributer& dist) {
    auto scope = juce::ScopedLock(dist.lock);
    dist.target_to_adapter.clear();
}
