#pragma once

#include <JuceHeader.h>
#include "../choc/gui/choc_WebView.h"


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

    void ResizeToComponent(juce::Component* component);
    void AddWebViewToComponent(juce::Component* component);
    // Sets the webview Html
    void SetHTML(const std::string& );
    // Sets the webview URL
    void SetURL(const std::string& );

private:
    #if JUCE_LINUX
    static unsigned long getWindowID (PatchWebView& v);
    #endif
    std::unique_ptr<choc::ui::WebView> m_webview;
    std::unique_ptr<NativeUIBase> m_nativeBase;
};
}
