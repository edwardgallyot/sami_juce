use cxx;

// Import message types
mod message_types;
mod message;

use message_types::{
    MessageType,
    get_message_cxx_type,
    set_message_cxx_type,
};

// Use our message module
use message::{
    Message,
    get_message_id,
    create_message_from_json,
    create_message,
    destroy_message,
    create_json_from_message
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
    enum CxxMessage {
        FloatUpdate,
        IntUpdate,
        BoolUpdate,
        GestureUpdate,
        Invalid
    }
    // All the functions that C++ need to work with message objects.
    extern "Rust" {
        // Define all the functions for a standard message in C++
        type Message;
        // Serialises a message from incoming JSON data.
        #[namespace="message"]
        #[cxx_name="CreateMessageFromJSON"]
        fn create_message_from_json(string: &CxxString) -> *mut Message;
        // Gets the unique id for the message.
        #[namespace="message"]
        #[cxx_name="GetMessageId"]
        fn get_message_id(message: &Message) -> Result<String>;
        // Creates an Invalid message type
        #[namespace="message"]
        #[cxx_name="CreateMessage"]
        fn create_message() -> *mut Message;
        // Serialises a message to be sent back as JSON data
        #[namespace="message"]
        #[cxx_name="CreateJSONFromMessage"]
        fn create_json_from_message(message: &Message) -> Result<String>;
        // Destroys a rust allocated message
        #[namespace="message"]
        #[cxx_name="DestroyMessage"]
        unsafe fn destroy_message(message: *mut Message);
        // Gets the message type from a 
        #[namespace="message"]
        #[cxx_name="GetMessageType"]
        fn get_message_cxx_type(message: &Message) -> CxxMessage;
        // Sets the type of message from C++
        #[namespace="message"]
        #[cxx_name="SetMessageType"]
        fn set_message_cxx_type(message: &mut Message, message_type: CxxMessage);
    }

    // TODO
    // IDS
    // Each Update should be targeted with an ID. These should be enumerated the same
    // in typescript and in Rust.
    #[repr(i32)]
    enum CxxID {

    }

    // TODO
    // UPDATES
    // Any update that is larger than needed we will have to allocate the memory for it before
    // we try and serialise it.
}


