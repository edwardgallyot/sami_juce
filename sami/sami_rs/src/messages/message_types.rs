use ts_rs::TS;
use serde::{Serialize, Deserialize};
use crate::{
    Message, 
    sami::CxxMessage
};
// This enum can be setup to keep any number of options
// it's up to us how we implement the C++ api.
#[derive(Serialize, Deserialize, Debug, Copy, Clone, TS)]
#[ts(export)]
pub enum MessageType {
    // Inits
    Init,
    // Updates
    FloatUpdate(f32),
    IntUpdate(i32),
    BoolUpdate(bool),
    GestureUpdate(bool),

    // Custom Types
}

pub fn get_message_cxx_type(message: &Message) -> CxxMessage {
    // If we don't create a Cxx Type for every typescript type
    // this function will give us a build warning!
    // This is exactly what we want, we don't want this to be failing at runtime...
    match message.message {
        Some(MessageType::Init) => CxxMessage::Init,
        Some(MessageType::FloatUpdate(..)) => CxxMessage::FloatUpdate,
        Some(MessageType::IntUpdate(..)) => CxxMessage::IntUpdate,
        Some(MessageType::BoolUpdate(..)) => CxxMessage::BoolUpdate,
        Some(MessageType::GestureUpdate(..)) => CxxMessage::GestureUpdate,
        None => CxxMessage::Invalid
    }
}
