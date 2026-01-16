#!/bin/bash
# Build and Install 909 Kick Builder VST3
# This script builds the plugin and copies it to your VST3 folder

set -e  # Exit on error

echo "🔨 Building 909 Kick Builder VST3..."

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Configure with CMake
echo "Configuring build..."
cmake -DCMAKE_BUILD_TYPE=Release -G "Xcode" ..

# Build the project
echo "Building project (this may take a few minutes)..."
cmake --build . --config Release

# Find the built VST3 plugin
VST3_PATH=$(find . -name "909 Kick Builder.vst3" -type d | head -1)

if [ -z "$VST3_PATH" ]; then
    echo "❌ Error: VST3 plugin not found after build"
    exit 1
fi

echo "✅ Build successful!"

# Copy to VST3 folder
TARGET_DIR="/Library/Audio/Plug-Ins/VST3"
echo "📦 Installing to ${TARGET_DIR}..."

# Remove old version if it exists
if [ -d "${TARGET_DIR}/909 Kick Builder.vst3" ]; then
    echo "Removing old version..."
    sudo rm -rf "${TARGET_DIR}/909 Kick Builder.vst3"
fi

# Copy new version
echo "Copying new version..."
sudo cp -r "${VST3_PATH}" "${TARGET_DIR}/"

echo "✅ Installation complete!"
echo "🎵 909 Kick Builder VST3 is now available in your DAW"
