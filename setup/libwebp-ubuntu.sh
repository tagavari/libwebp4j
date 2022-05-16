#!/bin/bash -e

# Install libwebp on Ubuntu.
# This script compiles from source because the prebuilt
# binaries are not compatible with dynamic libraries.

LIBWEBP_VER=$1
LIBWEBP_ARCHIVE="libwebp-$LIBWEBP_VER"
LIBWEBP_ZIP="$LIBWEBP_ARCHIVE.tar.gz"

#Pick random directories
BUILD_DIR=$(mktemp -d)
INSTALL_DIR=$(mktemp -d)

#Download and extract the release package
INSTALL_DIR=$(mktemp -d)
LIBWEBP_ARCHIVE_URL="https://storage.googleapis.com/downloads.webmproject.org/releases/webp/$LIBWEBP_ZIP"
wget -qO- "$LIBWEBP_ARCHIVE_URL" | tar xvz -C "$BUILD_DIR"

#Jump into a build directory
pushd "$BUILD_DIR/$LIBWEBP_ARCHIVE" || exit

#Configure
./configure --disable-dependency-tracking --disable-shared --with-libwebp --disable-libwebpmux --disable-libwebpdemux --prefix="$INSTALL_DIR"

#Make and install
make
make install

#Clean up
popd || exit
rm -rf "$BUILD_DIR"

#Record the original PATH
echo "ORIGINAL_PATH=$PATH" >> "$GITHUB_ENV"

#Add the directory to the PATH
echo "$INSTALL_DIR/lib" >> "$GITHUB_PATH"
echo "$INSTALL_DIR/include" >> "$GITHUB_PATH"
