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
class WebViewContainer 
{
public:
    WebViewContainer();
    ~WebViewContainer();
    juce::Component* GetComponent();
private:
    #if JUCE_LINUX
    static unsigned long getWindowID (PatchWebView& v);
    #endif
    std::unique_ptr<choc::ui::WebView> m_webview;
    std::unique_ptr<NativeUIBase> m_nativeBase;
};

}
