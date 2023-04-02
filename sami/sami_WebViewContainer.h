#pragma once

#include <JuceHeader.h>
#include "../choc/gui/choc_WebView.h"

// Macros defined in the same way as a generated CMajor project
// for more info see https://cmajor.dev
// Much like a generated CMajor project we use a choc::WebView object
// to generate the javascript ui.
// Unlike the webview and cmajor though, sami's custom fork will use protobuf
// to serialise the data.
#if JUCE_MAC
 using NativeUIBase = juce::NSViewComponent;
#elif JUCE_WINDOWS
 using NativeUIBase = juce::HWNDComponent;
#else
 using NativeUIBase = juce::XEmbedComponent;
#endif


namespace sami
{
// Deals with the cross-`platform webview things, should be owned by a webview component
class WebViewContainer 
{
public:
    WebViewContainer();
    ~WebViewContainer();

    // Resizes the webview in this container to the component supplied.
    void ResizeToComponent(juce::Component* component);
    // Adds the webview in this container to the component supplied.
    void AddWebViewToComponent(juce::Component* component);
    // Sets the webview in this containers HTML
    void SetHTML(const std::string& );
    // Sets the webview in this containers URL
    void SetURL(const std::string& );

private:
    #if JUCE_LINUX
    static unsigned long getWindowID (PatchWebView& v);
    #endif
    std::unique_ptr<choc::ui::WebView> m_webview;
    std::unique_ptr<NativeUIBase> m_nativeBase;
};
}
