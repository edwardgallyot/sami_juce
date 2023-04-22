use crate::Message;
use crate::messages::message_types::MessageType::IntUpdate;

pub fn set_int_update(message: &mut Message, value: i32) {
    message.message = Some(IntUpdate(value));
}

pub fn get_int_update(message: &Message, value: &mut i32) -> bool {
    if let Some(IntUpdate(inner)) = &message.message {
        *value = *inner;
        true
    } else {
        false
    }

}
