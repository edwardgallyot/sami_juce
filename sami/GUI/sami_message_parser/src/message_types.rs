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
    FloatUpdate(f32),
    IntUpdate(i32),
    BoolUpdate(bool),
    GestureUpdate(bool),
    Invalid
}

pub fn get_message_cxx_type(message: &Message) -> CxxMessage {
    // If we don't create a Cxx Type for every typescript type
    // this function will give us a build warning!
    // This is exactly what we want, we don't want this to be failing at runtime...
    match message.message {
        MessageType::FloatUpdate(..) => CxxMessage::FloatUpdate,
        MessageType::IntUpdate(..) => CxxMessage::IntUpdate,
        MessageType::BoolUpdate(..) => CxxMessage::BoolUpdate,
        MessageType::GestureUpdate(..) => CxxMessage::GestureUpdate,
        MessageType::Invalid => CxxMessage::Invalid,
    }
}

pub fn set_message_cxx_type(message: &mut Message, message_type: CxxMessage) {
    match message_type {
        CxxMessage::FloatUpdate => message.message = MessageType::FloatUpdate(0.0),
        CxxMessage::IntUpdate => message.message = MessageType::IntUpdate(0),
        CxxMessage::BoolUpdate => message.message = MessageType::BoolUpdate(false),
        CxxMessage::GestureUpdate => message.message = MessageType::GestureUpdate(false),
        CxxMessage::Invalid => message.message = MessageType::Invalid,
        _ => panic!("Unspported MessageType here!") // In reality this shouldn't happen since the
        // enums are pretty type safe however there's nothing really stopping someone doing
        // something dumb here...
    }
}
