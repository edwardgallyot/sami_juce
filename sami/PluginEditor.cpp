#include "GUI/sami_webview_adapter.hpp"
#include "GUI/sami_message_parser/target/cxxbridge/sami_message_parser/src/lib.rs.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "DSP/sami_params.hpp"

//==============================================================================
sami::Editor::Editor (AudioProcessor& proc)
    : 
    AudioProcessorEditor (proc), 
    p (proc),
    web(true),
    gain_adapter(
    )
{
    // Set up the webview component editor and add the webview component
    juce::ignoreUnused (p);
    this->setResizable(true, true);
    this->setSize(400, 400);
    this->addAndMakeVisible(web);
    this->web.webview_container->webview->navigate("http://localhost:5173");

    // Add the gain adapter
    auto lock = juce::ScopedLock(this->listenerLock);
    this->p.parameters.addParameterListener(sami::params::gain, &this->gain_adapter);
    this->web.listeners.insert({sami::messages::targets::cxx::gain, &this->gain_adapter});

    auto gain_rx = [&] (const Message& message) {
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
        };
    auto gain_tx = [&] () {
            using namespace sami::messages;

            // Create our message and set a target
            auto message = create();
            targets::set(*message, targets::cxx::gain);

            // Get our new_value
            auto* param = p.parameters.getParameter(params::gain);
            float new_val = param->convertFrom0to1(param->getValue());
            updates::set_float(*message, new_val);

            // Create our stringified payload
            auto payload = std::string(to_js_command(to_json(*message)).c_str());
            web.webview_container->webview->evaluateJavascript(payload);
            destroy(message);
        };

    this->gain_adapter.processor_callback = gain_tx;
    this->gain_adapter.webview_callback = gain_rx;
    this->gain_adapter.startTimer(3);
}

sami::Editor::~Editor()
{
    auto lock = juce::ScopedLock(this->listenerLock);
    this->p.parameters.removeParameterListener(sami::params::gain, &this->gain_adapter);
}

    
void sami::Editor::paint (juce::Graphics& )
{
    /* empty implementation */
}

void sami::Editor::resized()
{
    web.setBounds(getLocalBounds());
}
