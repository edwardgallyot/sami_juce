#include "DSP/sami_webview_adapter.hpp"
#include "DSP/sami_webview_adapter_distributor.hpp"
#include "GUI/sami_message_parser/target/cxxbridge/sami_message_parser/src/lib.rs.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "DSP/sami_params.hpp"

//==============================================================================
sami::Editor::Editor (AudioProcessor& proc)
    : 
    AudioProcessorEditor (proc), 
    p (proc),
    web(true)
{
    juce::ignoreUnused (p);
    web.webview_container->webview->listener.push_back(this);
    this->setResizable(true, true);
    this->setSize(400, 400);
    this->addAndMakeVisible(web);
    this->web.webview_container->webview->navigate("http://localhost:5173");

    // GAIN ADAPTER
    // ============================================
    this->gain_adapter = adapters::create_webview_adapter(
            [&] (const Message& message) {
                using namespace sami::messages;
                float new_val = 0.0f;
                if (updates::get_float(message, new_val)) {
                    std::cout << new_val << std::endl;
                    auto* param = this->p.parameters.getParameter(params::gain);
                    std::cout << param->getParameterID() << std::endl;
                    param->beginChangeGesture();
                    param->setValueNotifyingHost(param->convertTo0to1(new_val));
                    param->endChangeGesture();
                }
            },
            [&] () {
                using namespace sami::messages;

                // Create our message and set a target
                auto message = create();
                targets::set(*message, targets::cxx::gain);

                // Get our new_value
                auto* param = p.parameters.getParameter(params::gain);
                float new_val = param->convertFrom0to1(param->getValue());
                updates::set_float(*message, new_val);

                // Create our stringified payload
                auto payload = to_json(*message);
                web.webview_container->webview->evaluateJavascript(to_js_command(payload).c_str());
                destroy(message);
            }
    );
    sami::adapters::add_to_webview_distributor(
        this->p.webview_distributer,
        messages::targets::cxx::gain,
        this->gain_adapter.get()
    );      
}

sami::Editor::~Editor()
{
    adapters::clear_webview_distributor(p.webview_distributer);
    web.webview_container->webview->listener.clear();
}

void sami::Editor::on_webview_message(const std::string &msg) {
     try {
         auto message = messages::create(msg);
         auto target = messages::targets::get(*message);
         auto adapter = p.webview_distributer.target_to_adapter.find(target);
         if (adapter != p.webview_distributer.target_to_adapter.end()) {
             adapter->second->on_webview_message(*message);
         } 
         messages::destroy(message);
     } catch (std::exception& e){
         std::cout 
            << e.what()<< std::endl
            << "Serialisation Error!" << std::endl; 
     }
    
}
void sami::Editor::paint (juce::Graphics& )
{
    /* empty implementation */
}

void sami::Editor::resized()
{
    web.setBounds(getLocalBounds());
}
