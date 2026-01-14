# 909 Kick Builder - Build Instructions

## Prerequisites

### macOS Requirements
- macOS 11.0 (Big Sur) or later
- Xcode 12.5 or later with Command Line Tools
- CMake 3.22 or later

### Install Dependencies

1. **Install Xcode Command Line Tools:**
```bash
xcode-select --install
```

2. **Install CMake (via Homebrew):**
```bash
brew install cmake
```

## Building the Plugin

### Step 1: Clone and Setup

The JUCE framework is already included in this repository.

### Step 2: Generate Xcode Project

From the project root directory:

```bash
cmake -B Builds/MacOSX -G Xcode \
  -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" \
  -DCMAKE_OSX_DEPLOYMENT_TARGET=11.0
```

This creates a Universal Binary for both Apple Silicon and Intel Macs.

### Step 3: Build with Xcode

**Option A: Build from Xcode GUI**
```bash
open Builds/MacOSX/909_KICK_BUILDER.xcodeproj
```

Then in Xcode:
- Select the `909KickBuilder_VST3` or `909KickBuilder_Standalone` scheme
- Choose Product > Build (⌘B)

**Option B: Build from Command Line**
```bash
cmake --build Builds/MacOSX --config Release
```

### Step 4: Install the Plugin

After building, the VST3 plugin will be in:
```
Builds/MacOSX/909KickBuilder_artefacts/Release/VST3/
```

Copy it to your VST3 folder:
```bash
cp -r "Builds/MacOSX/909KickBuilder_artefacts/Release/VST3/909 Kick Builder.vst3" \
  ~/Library/Audio/Plug-Ins/VST3/
```

The Standalone app will be in:
```
Builds/MacOSX/909KickBuilder_artefacts/Release/Standalone/
```

## Testing in Ableton Live

1. Launch Ableton Live 12 Suite
2. Create a new MIDI track
3. Add "909 Kick Builder" as an instrument
4. Trigger notes (any MIDI note will trigger the kick)
5. Adjust parameters in real-time

## Development Build (Debug)

For debugging with Xcode:

```bash
cmake --build Builds/MacOSX --config Debug
```

Then you can attach the Xcode debugger to your DAW process or run the Standalone app through Xcode.

## Troubleshooting

### "Cannot open developer command line tools"
```bash
sudo xcode-select --reset
```

### "Code signing required"
If you get code signing errors, you can disable it temporarily for development:
- Open the Xcode project
- Select the target → Signing & Capabilities
- Uncheck "Automatically manage signing"
- Set "Code Signing Identity" to "Sign to Run Locally"

### Plugin doesn't appear in DAW
- Make sure you copied to the correct VST3 directory
- Restart your DAW
- Rescan plugins in your DAW's preferences

### Build errors
- Ensure JUCE submodule is present: `ls JUCE/` should show files
- Clean build: `rm -rf Builds/MacOSX && cmake -B Builds/MacOSX ...`

## Next Steps

Once built successfully:
1. Test all parameters work
2. Try the factory presets
3. Save custom presets
4. Test in different DAWs
5. Profile CPU usage

## File Locations

- **VST3 System:** `~/Library/Audio/Plug-Ins/VST3/`
- **User Presets:** `~/Music/909 Kick Builder/Presets/`
- **Log Files:** `~/Library/Logs/909 Kick Builder/`

---

**Happy kick drum building!** 🥁

For issues, please refer to the main README.md or open a GitHub issue.
