use crate::Message;
use crate::messages::message_types::MessageType::FloatUpdate;

pub fn set_float_update(message: &mut Message, value: f32) {
    message.message = Some(FloatUpdate(value));
}

pub fn get_float_update(message: &Message, value: &mut f32) -> bool {
    if let Some(FloatUpdate(inner)) = &message.message {
        *value = *inner;
        true
    } else {
        false
    }

}
