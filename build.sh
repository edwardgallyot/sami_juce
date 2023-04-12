WORKING_DIR = $(pwd)
cd ./sami/GUI/sami_message_parser/ 
cargo build
cargo test
cd ../../../
cmake -B build
cmake --build build
