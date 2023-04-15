#[allow(unused_must_use)]
fn main() {
    cxx_build::bridge("src/lib.rs").compile("sami_message_parser");
    println!("cargo:rerun-if-changed=src/lib.rs");
}
