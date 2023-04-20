use crate::Message;
use crate::messages::message_types::MessageType::GestureUpdate;

pub fn get_gesture_update(message: &Message, value: &mut bool) -> bool {
    if let Some(GestureUpdate(inner)) = &message.message {
        *value = *inner;
        true
    } else {
        false
    }
}
