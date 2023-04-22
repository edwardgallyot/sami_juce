use crate::Message;
use crate::messages::message_types::MessageType::BoolUpdate;

pub fn set_bool_update(message: &mut Message, value: bool) {
    message.message = Some(BoolUpdate(value));
}

pub fn get_bool_update(message: &Message, value: &mut bool) -> bool {
    if let Some(BoolUpdate(inner)) = &message.message {
        *value = *inner;
        true
    } else {
        false
    }
}
