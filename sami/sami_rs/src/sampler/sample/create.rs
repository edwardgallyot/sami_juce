use wmidi::Note;
use crate::sampler::files::*;
use std::sync::{Arc, Mutex};
use std::thread;

use super::Sample;
use std::collections::HashMap;

// These are our sampler building functions

fn add_sample_to_map<'a>(map: Arc<Mutex<HashMap<Note, Sample>>>, note: Note, data: &'static [u8]) {
    if let Some(sample) =  Sample::new(data) {
        let lock = &mut *map.lock().unwrap();
        println!("Loaded Note: {}", note);
        lock.insert(note, sample);
    }
}

fn create_load_handle<'a>(map: Arc<Mutex<HashMap<Note, Sample>>>, note: Note, data: &'static [u8]) ->  thread::JoinHandle<()> {
    thread::spawn(move ||{
        add_sample_to_map(map, note, data)
    })
}

pub fn create<'a>() -> HashMap<Note, Sample> {

    let mut load_handles = vec![];
    let samples = Arc::new(Mutex::new(HashMap::new()));

    // Create this sampler.
    load_handles.push(create_load_handle(Arc::clone(&samples), Note::C4, SUSTAIN_C4_V1_RR1));
    load_handles.push(create_load_handle(Arc::clone(&samples), Note::Db4, SUSTAIN_DB4_V1_RR1));
    load_handles.push(create_load_handle(Arc::clone(&samples), Note::D4, SUSTAIN_D4_V1_RR1));
    load_handles.push(create_load_handle(Arc::clone(&samples), Note::Eb4, SUSTAIN_EB4_V1_RR1));
    load_handles.push(create_load_handle(Arc::clone(&samples), Note::E4, SUSTAIN_E4_V1_RR1));
    load_handles.push(create_load_handle(Arc::clone(&samples), Note::F4, SUSTAIN_F4_V1_RR1));
    load_handles.push(create_load_handle(Arc::clone(&samples), Note::Gb4, SUSTAIN_GB4_V1_RR1));
    load_handles.push(create_load_handle(Arc::clone(&samples), Note::G4, SUSTAIN_G4_V1_RR1));

    for handle in load_handles {
        handle.join().unwrap();
    }

    let lock = Arc::try_unwrap(samples);
    if let Ok(lock) = lock {
        lock.into_inner().unwrap()
    } else {
        HashMap::new()
    }
}
