#include "sami_WebViewContainer.h"

sami::WebViewContainer::WebViewContainer()
{
    #if JUCE_LINUX
    m_webview = std::make_unique<choc::ui::WebView>();
    m_nativeBase = std::make_unique<NativeUIBase>(getWindowID (*webview), true, false);
    #elif JUCE_MAC
    m_nativeBase = std::make_unique<NativeUIBase>();
    m_nativeBase->setSize(200, 200);
    m_webview = std::make_unique<choc::ui::WebView>();
    m_nativeBase->setView (m_webview->getViewHandle());
    #elif JUCE_WINDOWS
    m_nativeBase = std::make_unique<NativeUIBase>();
    m_nativeBase->setSize(200, 200);
    m_webview = std::make_unique<choc::ui::WebView>();
    m_nativeBase->setHWND (m_webview->getViewHandle());
    #endif
}
#if JUCE_LINUX
static unsigned long sami::WebViewContainer::getWindowID (chod::ui::WebView& v);
{
    auto childWidget = GTK_WIDGET (v.getViewHandle());
    auto plug = gtk_plug_new (0);
    gtk_container_add (GTK_CONTAINER (plug), childWidget);
    gtk_widget_show_all (plug);
    return gtk_plug_get_id (GTK_PLUG (plug));
}
#endif

sami::WebViewContainer::~WebViewContainer()
{
   #if JUCE_MAC
    m_nativeBase->setView ({});
   #elif JUCE_WINDOWS
    m_nativeBase->setHWND ({});
   #elif JUCE_LINUX
    removeClient();
   #endif
}

void sami::WebViewContainer::ResizeToComponent(juce::Component* component)
{
    m_nativeBase->setBounds(component->getBounds());
   #if JUCE_MAC
    m_nativeBase->resizeViewToFit ();
   #elif JUCE_WINDOWS
    m_nativeBase->updateHWNDBounds();
   #elif JUCE_LINUX
    // Fuck knows what we're doing here...
   #endif
}

void sami::WebViewContainer::AddWebViewToComponent(juce::Component* component)
{
    component->addAndMakeVisible(*m_nativeBase);
}

void sami::WebViewContainer::SetHTML(const std::string& html)
{
    m_webview->setHTML(html);
}

void sami::WebViewContainer::SetURL(const std::string& url)
{
    m_webview->navigate(url);
}
