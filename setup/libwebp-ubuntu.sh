#!/bin/bash -e

# Install libwebp on Ubuntu.
# This script compiles from source because the prebuilt
# binaries are not compatible with dynamic libraries.

LIBWEBP_VER=$1

#Install build dependencies with apt
sudo apt install libjpeg-dev libpng-dev libtiff-dev libgif-dev

#Jump into a random empty directory
TEMP_DIR=$(mktemp -d)
pushd "$TEMP_DIR" || exit

#Clone repository
git clone --depth 1 --branch "v$LIBWEBP_VER" "https://chromium.googlesource.com/webm/libwebp"
cd libwebp

#Autogen
./autogen.sh

#Configure
./configure

#Make and install
make
sudo make install

#Clean up
popd || exit
rm -rf "$TEMP_DIR"
