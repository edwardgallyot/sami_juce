#include "sami_WebView.h"
#include "sami_message_parser/target/cxxbridge/sami_message_parser/src/lib.rs.h"
#include <memory>
#include <ostream>

sami::WebView::WebView(bool enableDevTools)
    : choc::ui::WebView({
        enableDevTools, 
        [](const std::string&){ return std::nullopt;}
    })
{
}

sami::WebView::~WebView()
{
}
void sami::WebView::handleWebviewInvocation(const std::string & msg)
{
    using namespace messages;
    std::cout << "In C++ land" << std::endl;

    auto* message = create(msg);
    auto type = message_types::get(*message);

    if (type == message_types::cxx_message::float_update) {
        std::cout << "Successfully parsed float update" << std::endl;
    }
    std::cout << "TS Message" << std::endl;
    std::cout << to_json(*message).c_str() << std::endl;

 
    auto* new_message = create();

    updates::set_float_update(*new_message, 0.1f);

    std::cout << "C++ Message" << std::endl;
    std::string serialised =  to_json(*new_message).c_str();
    std::cout << serialised << std::endl;
    evaluateJavascript("console.log(" + serialised + ")");
    messages::destroy(message);
    messages::destroy(new_message);
}
