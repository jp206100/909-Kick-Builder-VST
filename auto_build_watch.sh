#!/bin/bash
# Automatic Build & Install Watcher
# Watches for changes in Source/ and automatically rebuilds and installs

echo "👀 Watching for changes in Source/ directory..."
echo "Press Ctrl+C to stop"
echo ""

# Function to build and install
build_and_install() {
    echo ""
    echo "🔄 Change detected! Building..."
    ./build_and_install.sh
    echo ""
    echo "👀 Watching for more changes..."
}

# Check if fswatch is installed
if ! command -v fswatch &> /dev/null; then
    echo "❌ fswatch is not installed."
    echo "Install it with: brew install fswatch"
    exit 1
fi

# Watch the Source directory for changes
fswatch -o Source/ Assets/ CMakeLists.txt | while read event; do
    build_and_install
done
