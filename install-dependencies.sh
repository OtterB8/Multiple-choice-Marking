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

echo "Installing yarn..."
curl -sS https://dl.yarnpkg.com/debian/pubkey.gpg | sudo apt-key add -
echo "deb https://dl.yarnpkg.com/debian/ stable main" | sudo tee /etc/apt/sources.list.d/yarn.list
sudo apt update && sudo apt install yarn