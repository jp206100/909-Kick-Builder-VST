# Quick Start - Building and Installing

## One-Time Build & Install

To build and install the plugin once:

```bash
./build_and_install.sh
```

This will:
1. Build the VST3 plugin
2. Copy it to `/Library/Audio/Plug-Ins/VST3/`
3. Make it available in your DAW

## Automatic Rebuild on Changes (Recommended)

For automatic rebuilding whenever you make changes:

### First Time Setup:
Install fswatch (if not already installed):
```bash
brew install fswatch
```

### Start Auto-Watch:
```bash
./auto_build_watch.sh
```

This will:
- Watch for any changes in `Source/`, `Assets/`, or `CMakeLists.txt`
- Automatically rebuild and reinstall whenever you save changes
- Keep running until you press Ctrl+C

This makes iteration much faster - just save your changes and the plugin will automatically update in your VST folder!

## Requirements

- Xcode Command Line Tools (install with: `xcode-select --install`)
- CMake (install with: `brew install cmake`)
- For auto-watch: fswatch (install with: `brew install fswatch`)

## Troubleshooting

If the build fails:
1. Make sure you have Xcode installed
2. Ensure JUCE folder exists (should be cloned automatically)
3. Check that CMake is installed: `cmake --version`

## Manual Build (Advanced)

If you prefer to build manually:

```bash
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -G "Xcode" ..
cmake --build . --config Release
sudo cp -r "./909KickBuilder_artefacts/Release/VST3/909 Kick Builder.vst3" /Library/Audio/Plug-Ins/VST3/
```
