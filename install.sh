#!/bin/bash

set -e  # Exit on error

echo "🚀 Installing fdup..."

# Define version and download URL
VERSION="v1.0.0"
BINARY_URL="https://github.com/chungchihhan/find-duplicates/releases/download/${VERSION}/fdup"
INSTALL_DIR="/usr/local/bin"

# Create temporary directory
TMP_DIR=$(mktemp -d)
cd "$TMP_DIR"

# Download binary
echo "📥 Downloading fdup ${VERSION}..."
if ! curl -fsSL "$BINARY_URL" -o fdup; then
    echo "❌ Download failed"
    exit 1
fi

# Add execute permission
chmod +x fdup

# Test if it runs
if ! ./fdup 2>&1 | grep -q "Usage"; then
    echo "❌ Downloaded binary is not compatible with your system"
    exit 1
fi

# Install
echo "📦 Installing to ${INSTALL_DIR} (may need password)..."
sudo install -m 755 fdup "${INSTALL_DIR}/fdup"

# Cleanup
cd
rm -rf "$TMP_DIR"

echo "✅ fdup installed successfully!"
echo ""
echo "Usage:"
echo "  fdup /path/to/directory          # Scan single directory"
echo "  fdup -r /path/to/directory       # Scan recursively"