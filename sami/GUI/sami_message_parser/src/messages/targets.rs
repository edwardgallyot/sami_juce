use ts_rs::TS;
use serde::{Deserialize, Serialize};
use crate::sami::CxxTarget;
use super::Message;
use strum_macros::EnumIter;

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
pub mod setters;
