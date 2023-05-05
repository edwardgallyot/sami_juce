# Store the current directory
WORKING_DIR=$(pwd)
cd sami/sami_rs
cargo build
cargo test
cd "$WORKING_DIR" || exit 1
cmake  -B build
cmake --build build
