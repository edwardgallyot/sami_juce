use super::Target;
use crate::Message;

pub fn set_gain(message: &mut Message) {
    message.target = Some(Target::Gain);
}
pub fn set_sustain(message: &mut Message) {
    message.target = Some(Target::Sustain);
}

pub fn set_bypass(message: &mut Message) {
    message.target = Some(Target::Bypass);
}
