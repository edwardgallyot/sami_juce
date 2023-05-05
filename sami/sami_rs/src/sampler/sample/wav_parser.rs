use std::{str, error};
use byteorder::{ReadBytesExt, LittleEndian, BigEndian};


#[derive(Debug)]
#[allow(dead_code)]
struct Header<'a> {
    id: &'a str,
    size: i32,
    format: &'a str,
    sub_chunk_1_id: &'a str,
    sub_chunk_1_size: i32,
    audio_format: i16,
    num_channels: i16,
    sample_rate: i32,
    byte_rate: i32,
    blockalign: i16,
    bits_per_sample: i16,
    data_id: &'a str,
    data_size: i32,
    data_start: usize,
}

impl<'a> Header<'a> {
    pub fn new(bytes: &[u8]) -> Result<Header, Box<dyn error::Error>>{
        let mut offset: usize = 0;

        let id =  str::from_utf8(&bytes[offset..offset+4])?;
        offset += 4;

        let size = i32::from_le_bytes(bytes[offset..offset+4].try_into()?);
        offset += 4;

        let format = str::from_utf8(&bytes[offset..offset+4])?;
        offset += 4;

        let mut sub_chunk_1_id = str::from_utf8(&bytes[offset..offset+4])?;
        offset += 4;

        let mut sub_chunk_1_size = i32::from_le_bytes(bytes[offset..offset+4].try_into()?);
        offset += 4;

        // Skip over the junk chunk if necersarry
        if sub_chunk_1_id == "JUNK" {
            offset += sub_chunk_1_size as usize;      
            sub_chunk_1_id = str::from_utf8(&bytes[offset..offset+4])?;
            offset += 4;
            sub_chunk_1_size = i32::from_le_bytes(bytes[offset..offset+4].try_into()?);
            offset += 4;
        }

        let audio_format = i16::from_le_bytes(bytes[offset..offset+2].try_into()?);
        offset += 2;

        let num_channels = i16::from_le_bytes(bytes[offset..offset+2].try_into()?);
        offset += 2;

        let sample_rate = i32::from_le_bytes(bytes[offset..offset+4].try_into()?);
        offset += 4;

        let byte_rate = i32::from_le_bytes(bytes[offset..offset+4].try_into()?);
        offset += 4;

        let blockalign = i16::from_le_bytes(bytes[offset..offset+2].try_into()?);
        offset += 2;

        let bits_per_sample = i16::from_le_bytes(bytes[offset..offset+2].try_into()?);
        offset += 2;

        let data_id = str::from_utf8(&bytes[offset..offset+4])?;
        offset += 4;

        let data_size = i32::from_le_bytes(bytes[offset..offset+4].try_into()?);
        offset += 4;

        Ok(Header {
                id,
                size,
                format,
                sub_chunk_1_id,
                sub_chunk_1_size,
                audio_format,
                num_channels,
                sample_rate,
                byte_rate,
                blockalign,
                bits_per_sample,
                data_id,
                data_size,
                data_start: offset
            }
        )
    }

    #[allow(dead_code)]
    pub fn print_info(&self) {
        dbg!(self);
    }

    pub fn get_num_frames(&self) -> i32 {
        let bytes_per_sample= (self.bits_per_sample / 8) as i32;
        let samples_in_file = self.data_size / bytes_per_sample;
        let frames = samples_in_file / self.num_channels as i32;
        frames
    }

    pub fn get_bytes_per_sample(&self) -> usize {
        let bytes_per_sample= (self.bits_per_sample / 8) as usize;
        bytes_per_sample
    }
}

pub struct WavFile<'a> {
    header: Header<'a>,
    pub num_frames: i32,
    pub num_channels: i32,
    bytes_per_sample: usize,
    starting_byte: usize,
    bytes: &'a [u8],
}

impl<'a> WavFile<'a> {
    pub fn new(bytes: &[u8]) ->  Result<WavFile, Box<dyn error::Error>> {
        let header = Header::new(bytes)?;
        let num_frames = header.get_num_frames();
        let bytes_per_sample = header.get_bytes_per_sample();
        let num_channels = header.num_channels as i32;
        let starting_byte = header.data_start;
        Ok(WavFile {
            header,
            num_frames,
            num_channels,
            bytes_per_sample,
            starting_byte,
            bytes,
        })
    }

    // This is useful for debugging a header file so we allow it to sit here
    // unused.
    #[allow(dead_code)]
    pub fn print_header(&self) {
        self.header.print_info();
        println!("Num Frames: {}", self.num_frames);
        println!("Num Frames: {}", self.bytes_per_sample);
    }
    
    pub fn get_stereo_sample(&self, position: usize) -> Result<(f32, f32), Box<dyn error::Error>> {
        if self.num_channels != 2 {
            return Err("Not stereo File!!".to_string().into());
        }
        let mut offset = position * self.bytes_per_sample;
        offset += self.starting_byte;

        let left: f32 = f32::from_le_bytes(self.bytes[offset..offset+self.bytes_per_sample].try_into()?);
        let right: f32 = f32::from_le_bytes(self.bytes[offset..offset+self.bytes_per_sample].try_into()?);
        Ok((left, right))
    }
}
