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
    std::cout << "In C++ land" << std::endl;

    auto* message = message::CreateMessageFromJSON(msg);
    auto type = message::GetMessageType(*message);
    if (type == CxxMessage::FloatUpdate) {
        std::cout << "Successfully parsed float update" << std::endl;
        
    }
    std::cout << "TS Message" << std::endl;
    std::cout << message::CreateJSONFromMessage(*message).c_str() << std::endl;

 
    auto* new_message = message::CreateMessage();
    message::SetMessageType(*new_message, sami::CxxMessage::FloatUpdate);

    std::cout << "C++ Message" << std::endl;
    std::string serialised =  message::CreateJSONFromMessage(*message).c_str();
    std::cout << serialised << std::endl;
    evaluateJavascript("console.log(" + serialised + ")");
    message::DestroyMessage(message);
    message::DestroyMessage(new_message);
}
