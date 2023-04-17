#include "sami_WebViewContainer.h"
#include <optional>

sami::WebViewContainer::WebViewContainer(bool enableDevTools)
{
    #if JUCE_LINUX
    this->webview = std::make_unique<sami::WebView>(enableDevTools);
    this->nativeBase = std::make_unique<NativeUIBase>(getWindowID (*webview), true, false);
    #elif JUCE_MAC
    this->nativeBase = std::make_unique<NativeUIBase>();
    this->nativeBase->setSize(200, 200);
    this->webview = std::make_unique<sami::WebView>(enableDevTools);
    this->nativeBase->setView (this->webview->getViewHandle());
    #elif JUCE_WINDOWS
    this->nativeBase = std::make_unique<NativeUIBase>();
    this->nativeBase->setSize(200, 200);
    this->webview = std::make_unique<sami::WebView>(enableDevTools);
    this->nativeBase->setHWND (this->webview->getViewHandle());
    #endif
}

sami::WebViewContainer::~WebViewContainer()
{
   #if JUCE_MAC
    this->nativeBase->setView ({});
   #elif JUCE_WINDOWS
    this->nativeBase->setHWND ({});
   #elif JUCE_LINUX
    removeClient();
   #endif
}

void sami::WebViewContainer::ResizeToComponent(juce::Component* component)
{
    this->nativeBase->setBounds(component->getBounds());
   #if JUCE_MAC
    this->nativeBase->resizeViewToFit ();
   #elif JUCE_WINDOWS
    this->nativeBase->updateHWNDBounds();
   #elif JUCE_LINUX
    // Fuck knows what we're doing here...
   #endif
}

void sami::WebViewContainer::AddWebViewToComponent(juce::Component* component)
{
    component->addAndMakeVisible(*this->nativeBase);
}
