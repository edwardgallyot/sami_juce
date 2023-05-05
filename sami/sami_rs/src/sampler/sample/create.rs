use wmidi::Note;
use crate::sampler::files::*;

use super::Sample;
use std::collections::HashMap;

// These are our sampler building functions

fn add_sample_to_map<'a>(map: &mut HashMap<Note, Sample<'a>>, note: Note, data: &'a [u8]) {
    if let Some(sample) =  Sample::new(data) {
        map.insert(note, sample);
    }
}

pub fn create<'a>() -> HashMap<Note, Sample<'a>> {
    let mut samples = HashMap::new();

    // Create this sampler.
    add_sample_to_map(&mut samples, Note::C4, SUSTAIN_C4_V1_RR1);
    add_sample_to_map(&mut samples, Note::Db4, SUSTAIN_DB4_V1_RR1);
    add_sample_to_map(&mut samples, Note::D4, SUSTAIN_D4_V1_RR1);
    add_sample_to_map(&mut samples, Note::Eb4, SUSTAIN_EB4_V1_RR1);
    add_sample_to_map(&mut samples, Note::E4, SUSTAIN_E4_V1_RR1);
    add_sample_to_map(&mut samples, Note::F4, SUSTAIN_F4_V1_RR1);
    add_sample_to_map(&mut samples, Note::Gb4, SUSTAIN_GB4_V1_RR1);
    add_sample_to_map(&mut samples, Note::G4, SUSTAIN_G4_V1_RR1);

    samples
}
