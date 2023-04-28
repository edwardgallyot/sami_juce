use super::Target;
use crate::Message;

pub fn set_gain(message: &mut Message) {
    message.target = Some(Target::Gain);
}
pub fn set_attack(message: &mut Message) {
    message.target = Some(Target::Attack);
}
pub fn set_decay(message: &mut Message) {
    message.target = Some(Target::Decay);
}
pub fn set_sustain(message: &mut Message) {
    message.target = Some(Target::Sustain);
}
pub fn set_release(message: &mut Message) {
    message.target = Some(Target::Release);
}
pub fn set_bypass(message: &mut Message) {
    message.target = Some(Target::Bypass);
}
