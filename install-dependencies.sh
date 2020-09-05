#!/bin/bash
echo "Installing required packages"
apt-get install build-essential
apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev

echo "Getting OpenCV Source Code from Github repo..."
git clone https://github.com/opencv/opencv.git

echo "Building OpenCV from Source Using CMake..."
cd opencv
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local ..
make -j4
make install

echo "Installing node..."
apt-get update
apt-get install nodejs

echo "Installing yarn..."
curl -sS https://dl.yarnpkg.com/debian/pubkey.gpg | sudo apt-key add -
echo "deb https://dl.yarnpkg.com/debian/ stable main" | sudo tee /etc/apt/sources.list.d/yarn.list
apt update && sudo apt install yarn