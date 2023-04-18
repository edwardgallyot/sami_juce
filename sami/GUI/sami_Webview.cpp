#include "sami_WebView.h"
#include <algorithm>
#include <exception>
#include <memory>
#include <ostream>
#include <ranges>
#include "../GUI/sami_message_parser/target/cxxbridge/sami_message_parser/src/lib.rs.h"

sami::WebView::WebView(bool enableDevTools)
    : choc::ui::WebView({
        enableDevTools, 
        [](const std::string&){ return std::nullopt;}
    })
{
    auto init = std::string(messages::inits::get_init_script().c_str());
    addInitScript(init);
}

sami::WebView::~WebView()
{
}
void sami::WebView::handleWebviewInvocation(const std::string & msg)
{
    std::for_each(
        listeners.begin(),
        listeners.end(),
        [&] (auto* l) {
            l->on_webview_message(msg);
        }
    );
}
