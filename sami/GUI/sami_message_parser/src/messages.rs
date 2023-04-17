// Modules
pub mod updates;
pub mod message_types;
pub mod targets;
pub mod inits;

use cxx::CxxString;
use serde::{Deserialize, Serialize};
use serde_json;
use ts_rs::TS;
pub use message_types::MessageType;
use targets::Target;

// A Unified Message Struct for use in Type script and C++
#[derive(Serialize, Deserialize, Debug, Clone, TS)]
#[ts(export)]
pub struct Message {
    pub target: Option<Target>,
    pub message: Option<MessageType>,
}

// A helper func to create the JS command we need from a stringified bit of rust JSON
pub fn create_js_command_from_string(command: &String) -> String {
    String::from("window.onPluginMessage.handle(".to_owned() + command + ");")
}

// We can get the message from the JSON string sent from the webview.
pub fn create_message_from_json(string: &CxxString) -> Result<*mut Message, serde_json::Error> {
    let json: Message = serde_json::from_str(&string.to_string())?;
    Ok(Box::into_raw(Box::new(json)))
}

pub fn create_message() -> *mut Message {
    Box::into_raw(Box::new(Message {
        target: None,
        message: None,
    }))
}

pub unsafe fn destroy_message(message: *mut Message) {
    let __box = Box::from_raw(message);
}

pub fn create_json_from_message(message: &Message) -> Result<String, serde_json::Error> {
    serde_json::to_string(&message)
}
