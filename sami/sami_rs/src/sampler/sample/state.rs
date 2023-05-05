// TODO: this will need to represent any state that the sample can be in,
// there's an opportunity for it to be in a nested enum if we want
// to think about ADSR
//
enum RoundRobin {
    One,
    Two,
    Three,
    Four,
    Five,
}

pub enum State {
    On,
    Off,
    RoundRobin(RoundRobin),
}
