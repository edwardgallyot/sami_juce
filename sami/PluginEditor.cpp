#include "GUI/sami_callbacks.hpp"
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
        web,
        proc,
        messages::targets::cxx::gain,
        params::gain
    )
{
    // Set up the webview component editor and add the webview component
    juce::ignoreUnused (p);
    this->setResizable(true, true);
    this->setSize(400, 400);
    this->addAndMakeVisible(web);
    this->web.webview_container->webview->navigate("http://localhost:5173");

    // Webview Callbacks
    {
        using namespace sami::callbacks::webview;
        register_callbacks_with_adapter(
            this->gain_adapter,
            maybe_send_float_update
        );
    }

    // Processor Callbacks
    {
        using namespace sami::callbacks::processor;
        register_callbacks_with_adapter(
            this->gain_adapter, 
            send_float_update
        );
    }
    // Create connections to adapters
    {
        auto lock = juce::ScopedLock(this->listenerLock);
        sami::adapters::register_adapters_with_listeners(
            &this->gain_adapter
        );
    }
    sami::adapters::start_adapter_timers(
        4,
        &this->gain_adapter
    );
}

sami::Editor::~Editor()
{
    // TODO: provide a remove listener lock
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
