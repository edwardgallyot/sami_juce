pub mod midi;
pub mod files;
pub mod sample;


pub use midi::process_midi;
use wmidi::Note;
use sample::{Sample, create_samples};
use std::collections::HashMap;

use self::sample::state::State;


// RUST SAMPLER
// ==========================
// This is the main sampler for sami
// TODO: Take into account sample rates from this level.
// Sami's samples will be at 48,000 Hz so we should account for the fact that 
// there could be other values of fs.
pub struct Sampler<'a> {
    samples: HashMap<Note, Sample<'a>>,
    fs: f64,
    block_size: usize,
    frame: i32,
}

// Here we get the implementation for sampler
impl<'a> Sampler<'a> {

    fn new(x: fn() -> HashMap<Note, Sample<'a>>, fs: f64, block_size: usize) -> Sampler<'a> {
        println!("num channels{}", block_size);
        Sampler {
            samples: x(),
            fs,
            block_size,
            frame: 0,
        }
    }

    fn process_frame(&mut self, mut audio_l: &mut f32, mut audio_r: &mut f32) {
        // For each sample we want to increment our sample by calling a stereo tick on the buffer
        // we're filling
        self.tick_stereo(&mut audio_l, &mut audio_r);
    }

    fn tick_stereo(&mut self, left: &mut f32, right: &mut f32){
        // Out is 0 for now...
        let mut out: f32 = 0.0;

        // Find out the state of our sample.
        for (_, sample) in &mut self.samples {
            if let State::On = sample.state {
                out += sample.get_next().0;
                out += sample.get_next().1;
            }
        }; 

        // Assign to our float references
        *left = out;
        *right = out;

        // Tick along a frame
        self.frame += 1;
        self.frame %= self.block_size as i32;
        
        // Make sure we haven't ticked beyond our block size
        assert!(self.frame < self.block_size as i32);
    }

    fn note_on(&mut self, note: Note) {
        if let Some(sample) = self.samples.get_mut(&note) {
            sample.activate();
        }
    }

    fn note_off(&mut self, note: Note) {
        if let Some(sample) = self.samples.get_mut(&note) {
            sample.reset();
        }
    }
}

// FFI functions between C++ and Rust 
// ====================================
// These guys are things we need for the C++ library and are exposed in lib.rs.
// They are unsafe so they can be called from within C++ but they mainly deref something
// and move back into safe rust.
pub unsafe fn create<'a>(fs: f64, block_size: usize) -> *mut Sampler<'a> {
    Box::into_raw(
        Box::new(
            Sampler::new(
                create_samples,
                fs,
                block_size   
            )
        )
    )
}

pub unsafe fn process_frame(sampler: &mut Sampler, audio_l: *mut f32, audio_r: *mut f32) {
    sampler.process_frame(
        &mut *audio_l,
        &mut *audio_r
     );
}

pub unsafe fn destroy(sampler: *mut Sampler) {
    let _scope = Box::from_raw(sampler);
}
