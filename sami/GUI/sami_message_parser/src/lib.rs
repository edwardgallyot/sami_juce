use cxx;

// Import module structure
mod messages;

use messages::message_types::{
    get_message_cxx_type,
};

// Use our message module
use messages::{
    Message,
    get_message_id,
    create_message_from_json,
    create_message,
    destroy_message,
    create_json_from_message
};

use messages::updates::{
    get_float_update,
    set_float_update,
};


// =================
// This is where the generated code from build.rs will come from
// this allows use to use rust code in C++ without hassle
#[cxx::bridge(namespace = "sami")]
mod sami {
    // MESSAGE NAMESPACE
    // Handles everything to do with serialising and deserialising a message from the
    // webview.

    // Since these types will be generated in typescript
    // it makes sense that we serialise them here and pass them
    // back to C++.
    
    // Expose a regular old enum to C++
    // We enforce that all these cases should be handled by matching all the arms
    // of our rust enum to the Cxx ones for the FFI, however, there's no guarantee
    // that we can't mess this up since all C++ is "unsafe"
    #[repr(i32)]
    #[namespace="sami::messages::message_types"]
    #[cxx_name="cxx_message"]
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
    // All the functions that C++ need to work with message objects.
    extern "Rust" {
        // Define all the functions for a standard message in C++
        type Message;
        // Serialises a message from incoming JSON data.
        #[namespace="sami::messages"]
        #[cxx_name="create"]
        fn create_message_from_json(json: &CxxString) -> *mut Message;
        // Gets the unique id for the message.
        #[namespace="sami::messages"]
        #[cxx_name="get_id"]
        fn get_message_id(message: &Message) -> Result<String>;
        // Creates an Invalid message type
        #[namespace="sami::messages"]
        #[cxx_name="create"]
        fn create_message() -> *mut Message;
        // Serialises a message to be sent back as JSON data
        #[namespace="sami::messages"]
        #[cxx_name="to_json"]
        fn create_json_from_message(message: &Message) -> Result<String>;
        // Destroys a rust allocated message
        #[namespace="sami::messages"]
        #[cxx_name="destroy"]
        unsafe fn destroy_message(message: *mut Message);
        // Gets the message type from a 
        #[namespace="sami::messages::message_types"]
        #[cxx_name="get"]
        fn get_message_cxx_type(message: &Message) -> CxxMessage;
    }

    // TODO
    // IDS
    // Each Update should be targeted with an ID. These should be enumerated the same
    // in typescript and in Rust.
    #[repr(i32)]
    enum CxxID {

    }
    extern "Rust" {

    }

    // TODO
    // UPDATES
    // Any update that is larger than needed we will have to allocate the memory for it before
    // we try and serialise it.
    extern "Rust" {
        #[namespace="sami::messages::updates"]
        fn set_float_update(message: &mut Message, value: f32);
        #[namespace="sami::messages::updates"]
        fn get_float_update(message: &Message, value: &mut f32) -> bool;
    }
}



