#include "GUI/sami_callbacks.hpp"
#include "GUI/sami_webview_adapter.hpp"
#include "sami_rs/target/cxxbridge/sami_rs/src/lib.rs.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "DSP/sami_params.hpp"

//==============================================================================
sami::Editor::Editor (AudioProcessor& proc)
    : 
    AudioProcessorEditor (proc), 
    p (proc),
    web(true),
    bypass_adapter(
        web,
        p,
        messages::targets::cxx::bypass,
        params::bypass
    ),
    gain_adapter(
        web,
        p,
        messages::targets::cxx::gain,
        params::gain
    ),
    sustain_adapter(
        web,
        p,
        messages::targets::cxx::sustain,
        params::sustain
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
            maybe_send_float_update,
            maybe_send_gesture_update
        );
        register_callbacks_with_adapter(
            this->sustain_adapter,
            maybe_send_float_update,
            maybe_send_gesture_update
        );
        register_callbacks_with_adapter(
            this->bypass_adapter,
            maybe_send_bool_update,
            maybe_send_gesture_update
        );
    }

    // Processor Callbacks
    {
        using namespace sami::callbacks::processor;
        register_callbacks_with_adapter(
            this->gain_adapter, 
            send_float_update
        );
        register_callbacks_with_adapter(
            this->sustain_adapter, 
            send_float_update
        );
        register_callbacks_with_adapter(
            this->bypass_adapter, 
            send_bool_update
        );
    }
    // Create connections to adapters
    {
        auto lock = juce::ScopedLock(this->listenerLock);
        sami::adapters::register_adapters_with_listeners(
            &this->gain_adapter,
            &this->sustain_adapter,
            &this->bypass_adapter
        );
    }
    sami::adapters::start_adapter_timers(
        5,
        &this->gain_adapter,
        &this->sustain_adapter,
        &this->bypass_adapter
    );
}

sami::Editor::~Editor()
{
    auto lock = juce::ScopedLock(this->listenerLock);
    adapters::remove_adapters_as_parameter_listeners(
        &this->gain_adapter,
        &this->sustain_adapter,
        &this->bypass_adapter
    );
    adapters::stop_adapter_timers(
        &this->gain_adapter,
        &this->sustain_adapter,
        &this->bypass_adapter
    );
}

    
void sami::Editor::paint (juce::Graphics& )
{
    /* empty implementation */
}

void sami::Editor::resized()
{
    web.setBounds(getLocalBounds());
}
