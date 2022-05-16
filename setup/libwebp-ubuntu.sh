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
LIBWEBP_ARCHIVE_URL="https://storage.googleapis.com/downloads.webmproject.org/releases/webp/$LIBWEBP_ZIP"
wget -qO- "$LIBWEBP_ARCHIVE_URL" | tar xvz -C "$BUILD_DIR"

#Jump into a build directory
pushd "$BUILD_DIR/$LIBWEBP_ARCHIVE" || exit

#Configure
./configure --disable-dependency-tracking --disable-shared --disable-libwebpmux --disable-libwebpdemux --prefix="$INSTALL_DIR"

#Make and install
make CFLAGS="-fPIC"
make install

#Clean up
popd || exit
rm -rf "$BUILD_DIR"

#Add the directory to the search paths
{
	echo "LIBWEBP_DIR=$INSTALL_DIR"
	echo "INCLUDE=$INSTALL_DIR/include"
	echo "LIB=$INSTALL_DIR/lib"
} >> "$GITHUB_ENV"
