use cxx;

// Import module structure
mod messages;

use messages::message_types::get_message_cxx_type;

use messages::targets::{
    set_message_cxx_target,
    get_message_cxx_target,
};

use messages::{
    Message,
    create_message_from_json,
    create_message,
    destroy_message,
    create_json_from_message,
    create_js_command_from_string,
};

use messages::updates::{
    get_float_update,
    set_float_update,
};

use messages::inits::get_init_script;


// This is where the generated code from build.rs will come from
// this allows use to use rust code in C++ to interface with Typescript.
#[cxx::bridge(namespace = "sami")]
mod sami {
    // MESSAGE NAMESPACE
    // Handles everything to do with serialising and deserialising a message from the
    // webview.

    // All the functions that C++ need to work with message objects.
    extern "Rust" {
        // Define all the functions for a standard message in C++
        type Message;
        // Serialises a message from incoming JSON data.
        #[namespace="sami::messages"]
        #[cxx_name="create"]
        fn create_message_from_json(json: &CxxString) -> Result<*mut Message>;
        // Creates an Invalid message type
        #[namespace="sami::messages"]
        #[cxx_name="create"]
        fn create_message() -> *mut Message;
        // Serialises a message to be sent back as JSON data
        #[namespace="sami::messages"]
        #[cxx_name="to_json"]
        fn create_json_from_message(message: &Message) -> Result<String>;
        
        #[namespace="sami::messages"]
        #[cxx_name="to_js_command"]
        fn create_js_command_from_string(command: &String) -> String;
        // Destroys a rust allocated message
        #[namespace="sami::messages"]
        #[cxx_name="destroy"]
        unsafe fn destroy_message(message: *mut Message);
    }

    // MESSAGE TYPES
    // Expose a regular old enum to C++
    // We enforce that all these cases should be handled by matching all the arms
    // in our rust enum to the Cxx ones for FFI.
    #[namespace="sami::messages::message_types"]
    #[cxx_name="cxx"]
    pub(crate) enum CxxMessage {
        #[cxx_name="float_update"]
        FloatUpdate,
        #[cxx_name="int_update"]
        IntUpdate,
        #[cxx_name="bool_update"]
        BoolUpdate,
        #[cxx_name="gesture_update"]
        GestureUpdate,
        #[cxx_name="invalid"]
        Invalid
    }

    extern "Rust" {
        // Gets the message type from a message in C++
        // C++ just needs to know what type of message it's recieved so
        // it can handle it correctly when recieved from TS.
        #[namespace="sami::messages::message_types"]
        #[cxx_name="get"]
        fn get_message_cxx_type(message: &Message) -> CxxMessage;
    }

    // Targets
    // Each Update should be targeted with an ID. These should be enumerated the same
    // in typescript and C++.
    // In type script land this comes as a string from the JSON.
    // In C++ these are enumerated.
    // However they are both generated from the same rust Enum making the types consistent
    // in both languages.
    #[repr(i32)]
    #[namespace="sami::messages::targets"]
    #[cxx_name="cxx"]
    enum CxxTarget {
        #[cxx_name="gain"]
        Gain,
        #[cxx_name="sustain"]
        Sustain,
        #[cxx_name="invalid"]
        Invalid,
    }

    extern "Rust" {
        #[namespace="sami::messages::targets"]
        #[cxx_name="get"]
        fn get_message_cxx_target(message: &Message) -> CxxTarget;

        #[namespace="sami::messages::targets"]
        #[cxx_name="set"]
        fn set_message_cxx_target(message: &mut Message, target: CxxTarget);
    }

    // INITS
    extern "Rust" {
        #[namespace="sami::messages::inits"]
        pub fn get_init_script() -> String;
    }

    // UPDATES
    // Any update that is larger than needed we will have to allocate the memory for it before
    // we try and serialise it.
    extern "Rust" {
        #[namespace="sami::messages::updates"]
        #[cxx_name="set_float"]
        fn set_float_update(message: &mut Message, value: f32);
        #[namespace="sami::messages::updates"]
        #[cxx_name="get_float"]
        fn get_float_update(message: &Message, value: &mut f32) -> bool;
    }
}

