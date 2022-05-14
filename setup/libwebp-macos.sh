#!/bin/zsh -e

# Install libwebp on macOS.
# Downloads and installs the specified prebuilt version from Google's servers.

LIBWEBP_VER=$1
LIBWEBP_ARCHIVE="libwebp-$LIBWEBP_VER-mac-x86-64"
LIBWEBP_ZIP="$LIBWEBP_ARCHIVE.tar.gz"

#Download and extract the release package
INSTALL_DIR=$(mktemp -d)
LIBWEBP_ARCHIVE_URL="https://storage.googleapis.com/downloads.webmproject.org/releases/webp/$LIBWEBP_ZIP"
curl --location "$LIBWEBP_ARCHIVE_URL" | tar xvz -C "$INSTALL_DIR"

#Add the directory to the PATH
echo "$INSTALL_DIR/$LIBWEBP_ARCHIVE/lib" >> $GITHUB_PATH
echo "$INSTALL_DIR/$LIBWEBP_ARCHIVE/include" >> $GITHUB_PATH
