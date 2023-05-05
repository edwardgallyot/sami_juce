mod create;
mod wav_parser;
pub mod state;

// Re-export the create function as create_samples
// in another project that uses other samples we can
// afford to adapt this code.
pub use create::create as create_samples; 

use self::{state::State, wav_parser::WavFile};

pub struct Sample<'a> {
    file: WavFile<'a>,
    pub state: State,
    phase: f32,
}


impl<'a> Sample<'a> {
    // Creates a new sample from the data supplied. 
    // If the sample can't be read from the bytes then no sample will be
    // returned.
    pub fn new(bytes: &[u8]) -> Option<Sample> {
        let file = WavFile::new(bytes);
        match file {
            Ok(file) => {
                return Some(Sample {
                    file,
                    state: State::Off,
                    phase: 0.0,
                });
            } Err(e) => {
                dbg!("Error reading bytes: {}", e.to_string());
                return None;
            }
        }
    }

    pub fn get_next(&mut self) -> (f32, f32){
        let sample = self.file.get_stereo_sample(self.phase as usize);
        match sample {
            Ok(sample) => {
                self.phase += 1.0;
                if self.phase > self.file.num_frames as f32 {
                    self.phase = 0.0;
                    self.state = State::Off;
                }
                return sample;
            }
            Err(e) => {
                dbg!("Error reading stereo file: {}", e.to_string());
                return (0.0, 0.0);
            }
        }
    }
    
    pub fn reset(&mut self) {
        self.phase = 0.0;
        self.state = State::Off;
    }

    pub fn activate(&mut self) {
        // Set the state to on.
        self.state = State::On;
        self.phase = 0.0;
    }
}
