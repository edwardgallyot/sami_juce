use serde::{Serialize, Deserialize};
use ts_rs::TS;
use serde_json;
use cxx::CxxString;
use crate::MessageType;

// A Unified Message Struct for use in Type script and C++
#[derive(Serialize, Deserialize, Debug, Clone, TS)]
#[ts(export)]
pub struct Message {
    pub id: Option<String>,
    pub message: MessageType
}

pub fn get_message_id(message: &Message) -> Result<String, String> {
    if let Some(id) =  message.id.clone() {
        Ok(id)
    } else {
        Err(String::from("Failed To Parse ID"))
    }
}

// We can get the message from the JSON string sent from the webview.
pub fn create_message_from_json(string: &CxxString) -> *mut Message {
    let json: Message = serde_json::from_str(&string.to_string()).unwrap();
    Box::into_raw(Box::new(json))
}

pub fn create_message() -> *mut Message {
    Box::into_raw(Box::new(Message { id: None, message: MessageType::Invalid}))
}


pub unsafe fn destroy_message(message: *mut Message) {
    let __box = Box::from_raw(message);
}

pub fn create_json_from_message(message: &Message) -> Result<String, serde_json::Error> {
    serde_json::to_string(&message)
}
