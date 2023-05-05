use wmidi::MidiMessage;
use core::slice;
use super::Sampler;

// Handle midi messages in idiomatic rust.
fn handle_midi_messages(sampler: &mut Sampler, bytes: &[u8], sample_position: i32) -> Result<(), wmidi::FromBytesError> {
    let message = MidiMessage::try_from(bytes)?;
    if sampler.frame == sample_position {
        match message {
            MidiMessage::NoteOn(_channel, note, _velocity) => {
                sampler.note_on(note);
            }
            MidiMessage::NoteOff(_channel, note, _velocity) => {
                sampler.note_off(note);
            }
            _ => todo!()
        }
    } 
    Ok(())
}

// FFI functinos for midi.
// ========================
pub unsafe fn process_midi(sampler: &mut Sampler, data: *const u8, num_bytes: usize, sample_position: i32) {
    let midi = slice::from_raw_parts(data, num_bytes);
    let result = handle_midi_messages(sampler, midi, sample_position);
    if let Err(_) = result {
        println!("Error handling midi messages");
    }
}
