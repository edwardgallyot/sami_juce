WORKING_DIR=$(pwd)
cargo build
cargo test
cd "$WORKING_DIR" || exit 1
cmake -B build
cmake --build build
