#!/bin/bash
echo "Getting OpenCV Source Code from Github repo..."
git clone https://github.com/opencv/opencv.git

echo "Building OpenCV from Source Using CMake..."
cd opencv
mkdir Building
cd build
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local ..
make -j4
make install