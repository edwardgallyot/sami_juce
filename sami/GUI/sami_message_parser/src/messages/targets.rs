use ts_rs::TS;
use serde::{Deserialize, Serialize};
use crate::sami::CxxTarget;
use super::Message;
use strum_macros::EnumIter;


#[derive(Serialize, Deserialize, PartialEq, Eq, Debug, Clone, TS)]
#[ts(export)]
pub struct Empty {
}

#[derive(EnumIter,PartialEq, Eq, Serialize, Deserialize, Debug, Clone, TS)]
#[ts(export)]
pub enum Target{
    Gain,
    Sustain,
}

pub fn get_message_cxx_target(message: &Message) -> CxxTarget {
    match message.target {
        Some(Target::Gain) => CxxTarget::Gain,
        Some(Target::Sustain) => CxxTarget::Sustain,
        None => CxxTarget::Invalid,
    }
}

pub fn set_message_cxx_target(message: &mut Message, target: CxxTarget) {
    match target {
        CxxTarget::Gain => message.target = Some(Target::Gain),
        CxxTarget::Sustain => message.target = Some(Target::Sustain),
        CxxTarget::Invalid => message.target = None,
        _ => message.target = None
    }
}
