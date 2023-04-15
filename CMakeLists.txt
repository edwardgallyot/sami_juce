# sami CMakeLists.txt

cmake_minimum_required(VERSION 3.9)

# Building our rust messaging library
set (SAMI_MESSAGE_CARGO_MANIFEST ${CMAKE_SOURCE_DIR}/sami/GUI/sami_message_parser/Cargo.toml)
set(SAMI_MESSAGE_CARGO_TARGET_DIRECTORY ${CMAKE_SOURCE_DIR}/sami/GUI/sami_message_parser/target)
set(SAMI_MESSAGE_SOURCE_FILE ${CMAKE_SOURCE_DIR}/sami/GUI/sami_message_parser/src/lib.rs)

# Store the generated CPP files as variables for linkage
set(SAMI_MESSAGE_BRIDGE_CPP ${SAMI_MESSAGE_CARGO_TARGET_DIRECTORY}/cxxbridge/sami_message_parser/src/lib.rs.cc)
set(SAMI_MESSAGE_INCLUDE_DIR ${SAMI_MESSAGE_BRIDGE_CPP}/..)
set(SAMI_MESSAGE_LIB ${SAMI_MESSAGE_CARGO_TARGET_DIRECTORY}/debug/${CMAKE_STATIC_LIBRARY_PREFIX}sami_message_parser${CMAKE_STATIC_LIBRARY_SUFFIX})

include(FetchContent)

FetchContent_Declare(
    Corrosion
    GIT_REPOSITORY https://github.com/corrosion-rs/corrosion.git
    GIT_TAG v0.3 # Optionally specify a commit hash, version tag or branch here
)

FetchContent_MakeAvailable(Corrosion)
corrosion_import_crate(MANIFEST_PATH ${SAMI_MESSAGE_CARGO_MANIFEST})

# This is for neovim :)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(sami VERSION 0.0.1)

# Latest C++ standard
set(CMAKE_CXX_STANDARD 23)

# We add our submodules here. We don't need to add choc as it's header only :)
add_subdirectory(JUCE)                    
link_directories(./sami/GUI/sami_message_parser/target/debug/)

# We may target pro tools here...but no point for now
# juce_set_aax_sdk_path(...)

juce_add_plugin(sami
    # VERSION ...                               # Set this if the plugin version is different to the project version
    # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    # ICON_SMALL ...
    # COMPANY_NAME ...                          # Specify the name of the plugin's author
    IS_SYNTH TRUE                       # Is this a synth or an effect?
    NEEDS_MIDI_INPUT TRUE               # Does the plugin need midi input?
    # NEEDS_MIDI_OUTPUT TRUE/FALSE              # Does the plugin need midi output?
    # IS_MIDI_EFFECT TRUE/FALSE                 # Is this plugin a MIDI effect?
    # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
    # COPY_PLUGIN_AFTER_BUILD TRUE/FALSE        # Should the plugin be installed to a default location after building?
    PLUGIN_MANUFACTURER_CODE Juce               # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE Dem0                            # A unique four-character plugin id with exactly one upper-case character
                                                # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
    FORMATS AU VST3 Standalone                  # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PRODUCT_NAME "sami")        # The name of the final executable, which can differ from the target name

# Generate the JUCE header
juce_generate_juce_header(sami) 

target_sources(sami
    PRIVATE
        sami/PluginEditor.cpp
        sami/PluginProcessor.cpp
        # sami GUI components
        sami/GUI/sami_WebViewContainer.cpp
        sami/GUI/sami_WebViewComponent.cpp
        sami/GUI/sami_WebView.cpp

        ${SAMI_MESSAGE_BRIDGE_CPP}
)


target_compile_definitions(sami
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0
        DONT_SET_USING_JUCE_NAMESPACE=1)

# We will end up adding our inlined assets as binary data from the html into juce
# juce_add_binary_data(AudioPluginData SOURCES ...)

# Here we link JUCE and protobuf
target_link_libraries(sami
        # Target JUCE libraries
    PRIVATE
        # AudioPluginData           # If we'd created a binary data target, we'd link to it here
        juce::juce_audio_utils
    PUBLIC
        # Target JUCE libraries
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags

        sami_message_parser
)