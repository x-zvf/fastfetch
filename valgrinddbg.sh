#!/bin/env sh

# make the script exit if any of the commands fail,
# making fastfetch not run if the build failed.
set -e

mkdir -p build/
cd build/
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --target fastfetch -j$(nproc)
valgrind --leak-check=full \
    --show-leak-kinds=all \
    --track-origins=yes \
    --verbose \
    --log-file=valgrind-out.txt \
    ./fastfetch
